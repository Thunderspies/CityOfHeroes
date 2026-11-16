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
			PositionOffset	0.27 0.06 0.1
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
			PositionOffset	0.26 0.052 0.145
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
			PositionOffset	0.25 0.040 0.18
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
			PositionOffset	-0.29 0.078 0.04
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
			PositionOffset	-0.27 0.06 0.1
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
			PositionOffset	-0.26 0.052 0.145
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
			PositionOffset	-0.25 0.040 0.18
		End
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

#############################################################

End