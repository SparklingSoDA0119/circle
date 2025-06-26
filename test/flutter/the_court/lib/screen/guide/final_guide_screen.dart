import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:the_court/datas/dev_start_data.dart';
import 'package:the_court/screen/contents/home_screen.dart';
import 'package:the_court/screen/guide/layout/guide_layout.dart';

class FinalGuideScreen extends StatelessWidget {
  final DevStartData data;

  const FinalGuideScreen({
    super.key,
    required this.data,
  });

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(),
      body: GuideBodyLayout(
        mainTxt: '모든 과정을\n마쳤어요.',
        subTxt: '즐거운 운동을 시작해요.',
        nxtBtnTxt: '시작하기',
        onNxtBtnPressed: () async {
          bool isSaved = await data.save();
          if (!isSaved) {
            return;
          }

          Navigator.of(context).pushAndRemoveUntil(
            MaterialPageRoute(
              builder: (_) => HomeScreen(
                data: data,
              ),
            ),
            (Route<dynamic> route) => false,
          );
        },
      ),
    );
  }
}
