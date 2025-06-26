import 'package:flutter/material.dart';
import 'package:the_court/datas/dev_start_data.dart';
import 'package:the_court/screen/guide/gender_screen.dart';
import 'package:the_court/screen/guide/greeting_screen.dart';
import 'package:the_court/screen/guide/layout/guide_layout.dart';

class NameScreen extends StatefulWidget {
  const NameScreen({super.key});

  @override
  State<NameScreen> createState() => _NameScreenState();
}

class _NameScreenState extends State<NameScreen> {
  String? name;
  final _formKey = GlobalKey<FormState>();

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(),
      body: GuideBodyLayout(
        mainTxt: '이름을\n알려주세요.',
        nxtBtnTxt: '다음',
        onNxtBtnPressed: (){
          final formKeyState = _formKey.currentState!;
          if (formKeyState.validate()) {
            formKeyState.save();
          }

          DevStartData data = DevStartData();
          data.name = name!;

          Navigator.of(context).push(
            MaterialPageRoute(
              builder: (_) => GenderScreen(
                data: data,
              ),
            )
          );
        },
        top: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            _topBody(),
          ],
        ),
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
            maxLength: 10,
            decoration: const InputDecoration(
              counterText: '',
              labelText: '이름',
            ),
            autovalidateMode: AutovalidateMode.onUserInteraction,
            validator: (value) {
              if (value!.isEmpty) {
                return '1자 이상 입력해주세요';
              }
              else if (value!.length > 10) {
                return '10자 내로 입력해주세요.';
              }

              return null;
            },
            onSaved: (value) {
              setState(() {
                name = value;
              });
            },
          ),
        )
      ],
    );
  }
}
