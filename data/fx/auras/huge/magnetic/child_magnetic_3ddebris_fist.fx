#########################################################
## Slime Aura Head
#########################################################

FxInfo

Flags DontSuppress
########################################################



## SOUND FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
#		Sound explohit 80 80 0.77
	End
End



Condition
	On	Time
	Time	1

	Event
		EName	Geo
		Type	Local

		BhvrOverride
          		PyrRotate  90 90 90
			PyrRotateJitter 180 180 180

			Scale 0.1 0.1 0.1

			Spin 0.05 0.05 0
			Spinjitter 0.025 0.01 0.02
         	End
		Geom	AuraGeoA
		Geom	AuraGeoB
#		Geom	AuraGeoC
#		Geom	AuraGeoD

#		Geom	AuraGeoE
#		Geom	AuraGeoF
	End
	Event
		EName	trail
		Type	local
		At	Geo
		altpiv 	1


		BhvrOverride
			Scale 0.01 0.01 0.1

         	End
		Part2	:Magnetic_SpeedTrail_Small.part
	End

End

End