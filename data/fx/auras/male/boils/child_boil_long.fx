#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################

#Condition
#	On	Cycle
#	Time	35
#	Chance	0.3
#
#	Event
#		EName	Head
#		Type	Local
#		At	Head
#		part	:Boils_Arms.part
#		Lifespan 	6
#		LifespanJitter	4
#	End
#End

Condition
	On	Time
	Time	0

	Event
		EName	Arm
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Boils_Arms_Long.part
	End
End

Condition
	On	Time
	Time	48

	Event
		EName	Arm
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Toxic_Splash_Droplets.part
		Part	:Toxic_Splash_Main.part
	End
End
#############################################################

End