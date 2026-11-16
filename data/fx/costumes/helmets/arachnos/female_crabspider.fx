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
			PositionOffset	0.23 0.06 0.04
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
			PositionOffset	0.22 0.045 0.09
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
			PositionOffset	0.20 0.025 0.13
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
			PositionOffset	0.18 0.015 0.16
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
			PositionOffset	-0.23 0.06 0.04
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
			PositionOffset	-0.22 0.045 0.09
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
			PositionOffset	-0.20 0.025 0.13
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
			PositionOffset	-0.18 0.015 0.16
		End
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

#############################################################

End