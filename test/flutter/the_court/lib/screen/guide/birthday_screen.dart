import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:the_court/datas/dev_start_data.dart';
import 'package:the_court/screen/guide/layout/guide_layout.dart';
import 'package:the_court/screen/guide/phone_number_screen.dart';

class BirthdayScreen extends StatefulWidget {
  final DevStartData data;

  BirthdayScreen({
    super.key,
    required this.data,
  });

  @override
  State<BirthdayScreen> createState() => _BirthdayScreenState();
}

class _BirthdayScreenState extends State<BirthdayScreen> {
  final _formKey = GlobalKey<FormState>(debugLabel: 'BirthDay');
  bool _isValidate = false;
  String? birthDayResult;


  @override
  Widget build(BuildContext context) {

    return Scaffold(
      appBar: AppBar(),
      body: GuideBodyLayout(
        mainTxt: '${widget.data.name} 님의\n생일을\n알려주세요.',
        top: _topBody(),
        nxtBtnTxt: '다음',
        onNxtBtnPressed: () {
          if (_formKey.currentState!.validate()) {
            _formKey.currentState!.save();
          }

          widget.data.birthDay = birthDayResult;

          Navigator.of(context).push(MaterialPageRoute(
            builder: (_) => PhoneNumberScreen(
              data: widget.data,
            ),
          ));
        },
      ),
    );
  }

  Widget _topBody() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.stretch,
      children: [
        Form(
          key: _formKey,
          child: TextFormField(
            autovalidateMode: AutovalidateMode.onUserInteraction,
            keyboardType: TextInputType.number,
            inputFormatters: [
              FilteringTextInputFormatter.digitsOnly,
              _HyphenFormatter(),
            ],
            decoration: InputDecoration(
              hintText: 'YYYYMMDD',
              labelText: '생년월일(YYYYMMDD)',
            ),
            validator: _birthDayValidator,
            onChanged: (value) {
              setState(() {
                _isValidate;
              });
            },
            onSaved: (value) {
              setState(() {
                birthDayResult = value;
              });
            },
          ),
        )
      ],
    );
  }

  String? _birthDayValidator(String? value) {
    bool isValidate = true;
    String? result;

    if (value!.isEmpty) {
      isValidate = false;
      result = '형식에 맞추어 입력하세요.';
    }
    else if (value!.isNotEmpty){
      if (value!.length == 10) {
        if (int.parse(value.substring(8, 10)) > 31) {
          isValidate = false;
          result = '날짜를 확인해주세요.';
        }
        else {
          DateTime dtResult = DateTime.parse(value);
          if (dtResult.year.toString() != (value.substring(0, 4)) ||
              dtResult.month != int.parse(value.substring(5, 7)) ||
              dtResult.day != int.parse(value.substring(8, 10))) {
            isValidate = false;
            result = '날짜를 확인해주세요.';
          }
          else {
            isValidate = true;
          }
        }
      }
      else {
        isValidate = false;
        result = '형식에 맞추어 입력하세요.';
      }
    }

    _isValidate = isValidate;
    return result;
  }
}


class _HyphenFormatter extends TextInputFormatter {
  @override
  TextEditingValue formatEditUpdate(TextEditingValue oldVal, TextEditingValue newVal) {
    final buffer = StringBuffer();
    final newText = newVal.text;
    final formattedText = newText.replaceAll(RegExp(r'[^0-9]'), '');

    if (newVal.text.length > 8) {
      return oldVal;
    }

    if (oldVal.text.length == 5 && newVal.text.length == 4) {
      buffer.write(oldVal.text.substring(0, 3));
    }
    else if (oldVal.text.length == 8 && newVal.text.length == 6) {
      buffer.write(oldVal.text.substring(0, 6));
    }
    else if (formattedText.length >= 4) {
      if (formattedText.length == 5) {
        if (int.parse(formattedText[4]) > 1) {

        }
      }
      for (int i = 0; i < formattedText.length; i++) {
        if (i == 4) {
          if (int.parse(formattedText[4]) > 1) {
            buffer.write('0');
          }
        }

        buffer.write(formattedText[i]);
        if (i == 3 || i == 5) {
          buffer.write('-');
        }
      }
    }
    else {
      return newVal;
    }

    return newVal.copyWith(
      text: buffer.toString(),
      selection: TextSelection.collapsed(
        offset: buffer.length,
      ),
    );
  }
}