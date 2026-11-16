#############################################################
## AgainstAllOdds_Activation.fx
#############################################################

FxInfo

Lifespan 150

#########################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Hips
		Sound against_all_odds 60 60 .8
	End

	Event
		Type	Local
		At	Hips
		Sound glow5_loop 60 60 .1
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 120
	End
End

#############################################################

#Condition
#	On 	Time
#	Time 	20
#
#	Event
#		EName	ChestOffset
#		Type	Local
#		At	Chest
#		Bhvr	Behaviors\GenericParticleFade.bhvr
#		BhvrOverride
#			PositionOffset		0 0.25 0
#		End
#		Part	:Shields_Activation_CoreGlow1.part
#		Part	:Shields_Activation_CoreStar1.part
#		pOther	Offset1
#		Lifespan 35
#	End
#End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Ename	BodyAura_Initial
		Type	Local
		At	Origin
		ChildFX	:Shields_Continuing_Pulse.fx
		Lifespan 60
	End
End

#############################################################

End