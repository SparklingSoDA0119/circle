import 'dart:io';

import 'package:shared_preferences/shared_preferences.dart';
import 'package:the_court/const/gender.dart';

class DevStartData
{
  String? _name;
  String? get name => _name;
  set name(String? val) => _name = val;
  final String nameKey = 'name';

  Gender? _gender;
  Gender? get gender => _gender;
  set gender(Gender? val) => _gender = val;
  final String genderKey = 'gender';

  String? _birthDay;
  String? get birthDay => _birthDay;
  set birthDay(String? val) => _birthDay = val;
  final String birthDayKey = 'birthDay';

  String? _phoneNum;
  String? get phoneNum => _phoneNum;
  set phoneNum(String? val) => _phoneNum = val;
  final String phoneNumKey = 'phoneNum';

  bool _isFirstRun = true;
  bool get isFirstRun => _isFirstRun;
  final String firstRunKey = 'isFirstRun';

  Future<DevStartData> load() async
  {
    final SharedPreferences prefs = await SharedPreferences.getInstance();

    _name = prefs.getString(nameKey);

    int? genderNum = prefs.getInt(genderKey);
    if (genderNum == 1) {
      _gender = Gender.Woman;
    }
    else {
      _gender = Gender.Man;
    }

    _birthDay = prefs.getString(birthDayKey);
    _phoneNum = prefs.getString(phoneNumKey);

    _isFirstRun = prefs.getBool(firstRunKey) ?? true;

    return this;
  }

  bool isValidate() {
    if (_name == null ||
        _gender == null ||
        _birthDay == null ||
        _phoneNum == null) {
      return false;
    }
    else {
      return true;
    }
  }

  Future<bool> save() async {
    if (!isValidate()) {
      return false;
    }

    final SharedPreferences prefs = await SharedPreferences.getInstance();

    prefs.setString(nameKey, _name!);

    int genderNum = _gender! == Gender.Man ? 0 : 1;
    prefs.setInt(genderKey, genderNum);

    prefs.setString(birthDayKey, _birthDay!);
    prefs.setString(phoneNumKey, _phoneNum!);
    prefs.setBool(firstRunKey, false);

    return true;
  }
}