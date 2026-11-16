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
			PositionOffset	0.19 0.145 0.285
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.2 0.165 0.22
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.22 0.175 0.15
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.22 0.19 0.065
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
			PositionOffset	-0.19 0.145 0.285
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.2 0.165 0.22
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.22 0.175 0.15
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.22 0.19 0.065
		End
		Part1	:WolfSpider_Glow1.part
	End
End


#############################################################

End