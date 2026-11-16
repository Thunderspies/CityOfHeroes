#############################################################
## ActiveDefense_Activation.fx
#############################################################

FxInfo

Lifespan 150

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound active_defense 60 60 .85
	End
End

#########################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	ChestOffset
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0.25 0
		End
		Part	:Shields_Activation_CoreGlow1.part
		Part	:Shields_Activation_CoreStar1.part
		Sound reconstruction 60 60 .0
		bhvr	behaviors\soundFade1.bhvr
		pOther	Offset1
		Lifespan 35
	End
End

#########################################################

#Condition
#	On	Time
#	Time	30
#
#	Event
#		Ename	BodyAura_Initial
#		Type	Local
#		At	Origin
#		ChildFX	:Shields_Continuing_Pulse.fx
#		Lifespan 60
#	End
#End

#############################################################

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	Hips
		Part5	POWERS\Regeneration\Glow01.part
		Part3	POWERS\Regeneration\ERing.part
		Part5	POWERS\Regeneration\ERing2.part
		Part1	POWERS\Regeneration\Cloud01.part
		Part2	POWERS\Regeneration\Cloud02.part
		Part4	POWERS\Regeneration\Cloud03.part
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	Hips
		Part3	POWERS\Regeneration\ERing.part
		Part5	POWERS\Regeneration\ERing2.part
		Part1	POWERS\Regeneration\Cloud01.part
		Part2	POWERS\Regeneration\Cloud02.part
		Part4	POWERS\Regeneration\Cloud03.part
	End
End

#############################################################

End