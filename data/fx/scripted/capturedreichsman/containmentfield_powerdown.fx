#############################################################
## ContainmentField_PowerDown.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On 	Time
	Time 	2

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0 8 0
		End
		Part	:DustRing1.part
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 14.5 0
		End
		Part	:DustRing2.part
		Lifespan 5
	End

	Event
		Type	Local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:DustRing2.part
		Lifespan 5
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	Root
		ChildFX	:Child_FieldFlashes.fx
		Lifespan 120
	End
End

#############################################################

Condition
	On 	Time
	Time 	50

	Event
		Type	Local
		At	Root
		ChildFX	:Child_ElectricalFlashes.fx
		Lifespan 110
	End
End

#############################################################

End