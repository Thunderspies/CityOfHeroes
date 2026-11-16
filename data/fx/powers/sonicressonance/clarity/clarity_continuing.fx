#############################################################
## Clarity - Continuing FX
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		#Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
		Sound maestro1 60 60 .33
	End
End

#########################################################

Condition
	On 	Time
	Time	0

	Event
		EName	HeadOffset
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset		0.0 0.1 0.1
		End
	End
End



Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At	HeadOffset
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
		BhvrOverride
			Alpha		255
			Scale		0.625 0.75 0.75
		End
		Geom	SonicBuff_Bubble2
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Neck
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Medium1.part
		POther	Hair
	End

	Event
		Type	Local
		At	HeadOffset
		part1	POWERS\SonicRessonance\Aura_SonicRipples_Head1.part
	End
End



#############################################################

End