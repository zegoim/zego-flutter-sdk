import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:example/pages/init_sdk_page.dart';
import 'package:example/config/zego_config.dart';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Zego Flutter Demo',
      theme: ThemeData(

        primarySwatch: Colors.blue,
      ),
      home: HomePage(title: '互动视频示例'),
    );
  }
}

class HomePage extends StatelessWidget {
  HomePage({Key key, this.title}) : super(key: key);

  final String title;

  @override
  Widget build(BuildContext context) {

    ZegoConfig config = new ZegoConfig();

    // TODO: implement build
    return Scaffold(
        appBar: AppBar(

          title: Text(title),
        ),
        body: SafeArea(
            child: Center(
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: <Widget>[
                  Text('基础功能'),
                  Padding(
                    padding: const EdgeInsets.only(bottom: 10.0),
                  ),
                  CupertinoButton(
                      color: Color(0xff0e88eb),
                      child: Text('推流'),
                      onPressed: () {
                        Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) {
                          return InitPage(true);
                        }));
                      }
                  ),
                  Padding(
                    padding: const EdgeInsets.only(bottom: 10.0),
                  ),
                  CupertinoButton(
                      color: Color(0xff0e88eb),
                      child: Text('拉流'),
                      onPressed: () {
                        Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) {
                          return InitPage(false);
                        }));
                      }
                  ),
                ],
              ),
            )
        )
    );
  }
}

/*class _HomePageState extends State<HomePage> {

  @override
  Widget build(BuildContext context) {

    return Scaffold(
      appBar: AppBar(

        title: Text(widget.title),
      ),
      body: SafeArea(
          child: Center(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[
                Text('基础功能'),
                Padding(
                  padding: EdgeInsets.only(bottom: 10.0),
                ),
                CupertinoButton(
                    color: Color(0xff0e88eb),
                    child: Text('推流'),
                    onPressed: () {

                    }
                ),
                Padding(
                  padding: EdgeInsets.only(bottom: 10.0),
                ),
                CupertinoButton(
                    color: Color(0xff0e88eb),
                    child: Text('拉流'),
                    onPressed: () {

                    }
                ),
              ],
            ),
          )
      )
    );
  }
}
*/
