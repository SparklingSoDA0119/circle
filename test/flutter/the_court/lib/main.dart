import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:the_court/datas/dev_start_data.dart';
import 'package:the_court/screen/contents/home_screen.dart';
import 'package:the_court/screen/guide/greeting_screen.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();

  SystemChrome.setPreferredOrientations(
    [
      DeviceOrientation.portraitUp,
      DeviceOrientation.portraitDown,
    ]
  );

  DevStartData data = await DevStartData().load();
  Widget? homeScreen;

  if (data.isFirstRun) {
    homeScreen = GreetingScreen();
  }
  else {
    homeScreen = HomeScreen(
      data: data,
    );
  }

  return runApp(
    MaterialApp(
      theme: ThemeData.light(
        useMaterial3: true,
      ),
      home: homeScreen,
    ),
  );
}