#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 8

		Event
			At Origin
			Type Local
			#Bhvr Behavior/genericparticlefade.bhvr
			BhvrOverride
				FadeOutLength 20
				#Scale .1 .1 .1
			End
			#Part POWERS\Flight\FlightCloud.part
			#Part POWERS\Flight\FlightCloudSteam.part
			#Part POWERS\Flight\FlightSteam.part
			Part POWERS\Flight\FlightCloudSteam2.part
			Part POWERS\Flight\FlightBodyMist.part
			#Geom Geo_FX_TestSphere
		End

	End

End
