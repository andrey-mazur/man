import QtQuick 2.0
import QtQuick.Controls 1.3
import manWaveform 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 250
    height: width

	ManWaveform {
		id: waveform
		anchors.fill: parent

		ShaderEffectSource {
			id: theSource
			sourceItem: waveform
			recursive: true
		}

		ShaderEffect {
			anchors.fill: waveform
			property variant source: theSource
			property real amplitude: 0.04 * wobbleSlider.value
			property real frequency: 20
			property real time: 0
			NumberAnimation on time { loops: Animation.Infinite; from: 0; to: Math.PI * 2; duration: 600 }
			fragmentShader:
				"uniform lowp float qt_Opacity;" +
				"uniform highp float amplitude;" +
				"uniform highp float frequency;" +
				"uniform highp float time;" +
				"uniform sampler2D source;" +
				"varying highp vec2 qt_TexCoord0;" +
				"void main() {" +
				"    highp vec2 p = sin(time + frequency * qt_TexCoord0);" +
				"    gl_FragColor = texture2D(source, qt_TexCoord0 + amplitude * vec2(p.y, -p.x)) * qt_Opacity;" +
				"}"
			Slider {
				id: wobbleSlider
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.bottom: parent.bottom
				height: 40
			}
		}
	}
}
