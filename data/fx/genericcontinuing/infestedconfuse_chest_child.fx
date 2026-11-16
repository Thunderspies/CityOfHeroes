#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	TumorGeo
		Type	Local
		At	Origin
		Bhvr	Auras\Male\Boils\Boil_Scale_Chest.bhvr
		BhvrOverride
			#TintGeom	1
			StartColor	47 243 250
		End
		Geom	FX_Tintable_Boil
		Lifespan 120
	End
End

#Condition
#	On	Time
#	Time	120
#
#	Event
#		EName	Arm
#		Type	Local
#		At	Origin
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		Part	:Infested_Splash_Droplets.part
#		Part	:Infested_Splash_Main.part
#	End
#End
#############################################################

End