import 'package:flutter/material.dart';
import 'package:the_court/screen/guide/layout/guide_layout.dart';
import 'package:the_court/screen/guide/name_screen.dart';

class GreetingScreen extends StatelessWidget {
  const GreetingScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(),
      body: GuideBodyLayout(
        mainTxt: '환영해요!',
        subTxt: 'Hello!',
        nxtBtnTxt: '시작하기',
        onNxtBtnPressed: () {
          Navigator.of(context).push(
            MaterialPageRoute(
              builder: (_) => NameScreen(),
            )
          );
        },
      ),
    );
  }
}
