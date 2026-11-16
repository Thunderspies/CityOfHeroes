#############################################################
## Wolf Spider Helmet - Glow
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.245 0.05 0.24
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.265 0.07 0.18
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.28 0.09 0.08
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.29 0.11 -0.015
		End
		Part1	:WolfSpider_Glow1.part
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.245 0.05 0.24
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.265 0.07 0.18
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.28 0.09 0.08
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.29 0.11 -0.015
		End
		Part1	:WolfSpider_Glow1.part
	End
End


#############################################################

End