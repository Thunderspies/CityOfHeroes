#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Eyes
		Bhvr	:CrabSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.32 0.035 -0.02
			Scale		1.5 1.5 1.5
		End
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	Eyes
		Bhvr	:CrabSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.30 0.0 0.04
		End
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Eyes
		Bhvr	:CrabSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.28 -0.03 0.08
		End
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Eyes
		Bhvr	:CrabSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.26 -0.05 0.12
		End
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Eyes
		Bhvr	:CrabSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.32 0.035 -0.02
			Scale		1.5 1.5 1.5
		End
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	Eyes
		Bhvr	:CrabSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.30 0.0 0.04
		End
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Eyes
		Bhvr	:CrabSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.28 -0.03 0.08
		End
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Eyes
		Bhvr	:CrabSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.26 -0.05 0.12
		End
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

#############################################################

End