#############################################################
## Frost.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On	Time
	Time	8

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part	:snowburst.part
		Part	:ColdArmMist.part
		Sound frost 75 75 1.0
		Lifespan 70
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part	:snowburst.part
		Part	:ColdArmMist.part
		Lifespan 70
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime
		Type	Local
		At	Mystic
		Part	:FrostIce.part
		Part	:FrostSnow.part
		Part	:FrostMist.part
		Lifespan 30
	End

	Event
		Ename	PushDirection
		Type	Posit
		At	Mystic
		Lookat	Target
		Lifespan 30
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 7.5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		50
			PhysForcePowerJitter	10
		End
		Lifespan 30
	End
End

#############################################################

End