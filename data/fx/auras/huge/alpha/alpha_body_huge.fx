#############################################################
## Alpha_Body_Male.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 0.3 0.0
		End
		part	Auras\Male\Alpha\LightRay_Up.part
		part	Auras\Male\Alpha\LightRay_Down.part
		part	Auras\Male\Alpha\LightRay_Right.part
		part	Auras\Male\Alpha\LightRay_Left.part

		part	Auras\Male\Alpha\Star_Body.part
		Part	Auras\Male\Alpha\Glow_Ring.part
		Part	:Glow_Large_Continuing.part
	End

End

#############################################################

End