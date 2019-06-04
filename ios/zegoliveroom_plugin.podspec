#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#
Pod::Spec.new do |s|
  s.name             = 'zegoliveroom_plugin'
  s.version          = '0.0.1'
  s.summary          = 'A flutter plugin which can use zego services.'
  s.description      = <<-DESC
A new flutter plugin project.
                       DESC
  s.homepage         = 'https://www.zego.im'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'Zego' => 'lizhanpeng@zego.im' }
  s.source           = { :path => './'}
  s.source_files = 'Classes/**/*'
  
  s.public_header_files = 'Classes/**/*.h'
  
  s.dependency 'Flutter'
  s.vendored_frameworks = 'ZegoLiveRoom.framework'
  s.ios.deployment_target = '8.0'
end
