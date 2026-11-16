#############################################################
## Alpha_Body_Male.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#############################################################


Condition
	On	Time
	Time	0

#############################################################

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.08
		End
		Part	:Star_Eyes.part
		Part	:Glow_Eyes.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.08
		End
		Part	:Star_Eyes.part
		Part	:Glow_Eyes.part
	End

#############################################################

	Event
		EName	Ring
		Type	Local
		At	WepL

		part	Auras\Male\Alpha\LightRay_Right_Hand.part
		part	Auras\Male\Alpha\LightRay_Left_Hand.part
		Part	Auras\Male\Alpha\Star_Hand.part
		Part	Auras\Male\Alpha\Star_Hand_Moving.part
		Part	Auras\Male\Alpha\Glow_Large_Continuing_Chest.part
		Part	Auras\Male\Alpha\Glow_Ring_Hand.part
	End

	Event
		EName	Ring
		Type	Local
		At	WepR

		part	Auras\Male\Alpha\LightRay_Right_Hand.part
		part	Auras\Male\Alpha\LightRay_Left_Hand.part
		Part	Auras\Male\Alpha\Star_Hand.part
		Part	Auras\Male\Alpha\Star_Hand_Moving.part
		Part	Auras\Male\Alpha\Glow_Large_Continuing_Chest.part
		Part	Auras\Male\Alpha\Glow_Ring_Hand.part
	End
End

#############################################################

End