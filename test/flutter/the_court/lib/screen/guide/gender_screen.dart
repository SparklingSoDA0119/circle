import 'package:flutter/material.dart';
import 'package:the_court/const/gender.dart';
import 'package:the_court/datas/dev_start_data.dart';
import 'package:the_court/screen/guide/birthday_screen.dart';
import 'package:the_court/screen/guide/layout/guide_layout.dart';

class GenderScreen extends StatefulWidget {
  final DevStartData data;
  const GenderScreen({
    super.key,
    required this.data,
  });

  @override
  State<GenderScreen> createState() => _GenderScreenState();
}

class _GenderScreenState extends State<GenderScreen> {
  Gender gender = Gender.Man;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(),
      body: GuideBodyLayout(
        mainTxt: '성별을\n알려주세요.',
        top: _topBody(),
        nxtBtnTxt: '다음',
        onNxtBtnPressed: () {
          widget.data.gender = gender;

          Navigator.of(context).push(
            MaterialPageRoute(
              builder: (_) => BirthdayScreen(
                data: widget.data,
              ),
            ),
          );
        },
      ),
    );
  }

  Widget _topBody()
  {
    return Column(
      children: [
        RadioListTile(
          title: Text('남자'),
          value: Gender.Man,
          groupValue: gender,
          onChanged: (value) {
            setState(() {
              gender = value!;
            });
          },
        ),
        RadioListTile(
          title: Text('여자'),
          value: Gender.Woman,
          groupValue: gender,
          onChanged: (value) {
            setState(() {
              gender = value!;
            });
          },
        ),
      ],
    );
  }
}
