import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:the_court/datas/dev_start_data.dart';
import 'package:the_court/screen/guide/final_guide_screen.dart';
import 'package:the_court/screen/guide/layout/guide_layout.dart';

class PhoneNumberScreen extends StatefulWidget {
  final DevStartData data;

  const PhoneNumberScreen({
    super.key,
    required this.data,
  });

  @override
  State<PhoneNumberScreen> createState() => _PhoneNumberScreenState();
}

class _PhoneNumberScreenState extends State<PhoneNumberScreen> {
  final _formKey = GlobalKey<FormState>();
  final _formKey2 = GlobalKey<FormState>();
  bool _doingAuth = false;
  String _authNum = '';

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(),
      body: GuideBodyLayout(
        mainTxt: '휴대전화를\n인증해주세요',
        nxtBtnTxt: '다음',
        onNxtBtnPressed: _formKey2.currentState == null ? null :
          _formKey2.currentState!.validate() ? () {

          if (_formKey.currentState!.validate()) {
            _formKey.currentState!.save();
          }

          Navigator.of(context).push(
            MaterialPageRoute(
              builder: (_) => FinalGuideScreen(
                data: widget.data,
              ),
            )
          );
        } : null,
        top: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            _phoneNumInputLayout(),
            _authLayout(),
          ],
        ),
      ),
    );
  }


  Widget _phoneNumInputLayout()
  {
    return Form(
      key: _formKey,
      child: Row(
        children: [
          SizedBox(
            width: MediaQuery.of(context).size.width * 0.5,
            child: TextFormField(
              autovalidateMode: AutovalidateMode.onUserInteraction,
              keyboardType: TextInputType.number,
              decoration: InputDecoration(
                labelText: '휴대전화 번호',
              ),
              inputFormatters: [
                FilteringTextInputFormatter.digitsOnly,
                _HyphenFormatter(),
              ],
              validator: _phoneNumValidator,
              onSaved: (value) {
                setState(() {
                  widget.data.phoneNum = value;
                });
              },
            ),
          ),
          SizedBox(
            width: 32.0,
          ),
          ElevatedButton(
            onPressed: _formKey.currentState == null ? null :
            _formKey.currentState!.validate() ? () {
              setState(() {
                _doingAuth = true;
              });


            } : null,
            style: ElevatedButton.styleFrom(
                padding: _doingAuth? EdgeInsets.symmetric(
                    horizontal: MediaQuery.of(context).size.width * 0.1
                ) : null,
                shape: RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(10.0),
                )
            ),
            child: _doingAuth ? Text('재발송') : Text('인증번호 요청'),
          ),
        ],
      ),
    );
  }


  Widget _authLayout()
  {
    return Row(
      children: [
        SizedBox(
          width: MediaQuery.of(context).size.width * 0.5,
          child: Form(
            key: _formKey2,
            child: TextFormField(
              keyboardType: TextInputType.number,
              autovalidateMode: AutovalidateMode.onUserInteraction,
              inputFormatters: [
                FilteringTextInputFormatter.digitsOnly,
              ],
              decoration: InputDecoration(
                labelText: '인증 번호',
              ),
              validator: (value) {
                if (value == null) {
                  return '인증번호를 입력하세요.';
                }
                else if (value.isEmpty) {
                  return '인증번호를 입력하세요.';
                }
                else if (value.length != 6) {
                  return '인증번호가 올바르지 않습니다.';
                }

                return null;
              },
              onSaved: (value) {

                if (value == null) {
                  return;
                }

                if (value.isEmpty) {
                  return;
                }

                setState(() {
                  _authNum = value;
                });
              },
            ),
          ),
        )
      ],
    );
  }

  String? _phoneNumValidator(String? value) {
    if (value == null || value!.isEmpty) {
      return '번호를 입력하세요';
    }

    if (value![0] != '0') {
      return '형식에 맞지 않습니다.';
    }

    return null;
  }
}

class _HyphenFormatter extends TextInputFormatter {
  @override
  TextEditingValue formatEditUpdate(TextEditingValue oldVal, TextEditingValue newVal) {
    debugPrint('old: ${oldVal.text}, new: ${newVal.text}');

    final buffer = StringBuffer();
    final newTxt = newVal.text;

    for (int i = 0; i < newTxt.length; i++) {
      if (i == 3 || i == 7) {
        buffer.write('-');
      }

      buffer.write(newTxt[i]);

    }

    if (buffer.isNotEmpty) {
      return newVal.copyWith(
        text: buffer.toString(),
        selection: TextSelection.collapsed(
          offset: buffer.length,
        )
      );
    }

    return newVal;
  }
}
