#########################################################
## Slime Aura Head
#########################################################

FxInfo

Flags DontSuppress
########################################################



## POWER FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SpinnerAnchor
		Type	Local
		At	Chest
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.15 0.0
		End
	End

	Event
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
			StartJitter	0.5 0.0 0.0
		End

		Part	:Magnetic_SpeedTrail.part
	End

#	Event
#		Type	Local
#		At	SpinnerAnchor
#		Bhvr	behaviors/GenericParticleFade.bhvr
#
#		part	:Electron_Trail_Stationary.part
#	End
End
#Condition
#	On	Time
#	Time	1
#	Repeat  10
#
#	Event
#		EName	HeadSlime
#		Type	start
#		At	chest
#
#		BhvrOverride
##			PositionOffset 41.0 1.5 1.0
##			PyrRotate  0 0  90
##			PyrRotateJitter 180 180 180
##
##
##			Scale 0.3 0.3 0.3
##
##			Spin 0.05 0 0
##			Spinjitter 0.025 0.01 0.02
#         	End
#		Part2	:Magnetic_SpeedTrail.part
##		Geom	AuraGeoA
##		Geom	AuraGeoB
##		Geom	AuraGeoC
##		Geom	AuraGeoD
##		Geom	AuraGeoE
##		Geom	AuraGeoF
#
##		Part2	:Magnetic_SpeedTrail.part
#
#	End
#
#End


End