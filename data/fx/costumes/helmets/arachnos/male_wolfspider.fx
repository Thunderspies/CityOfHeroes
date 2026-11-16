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
			PositionOffset	0.225 0.08 0.33
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.25 0.1 0.25
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.275 0.12 0.16
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	0.275 0.125 0.06
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
			PositionOffset	-0.225 0.09 0.33
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.25 0.1 0.25
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.275 0.1 0.16
		End
		Part1	:WolfSpider_Glow1.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:WolfSpider_EyeGlow1.bhvr
		BhvrOverride
			PositionOffset	-0.275 0.125 0.06
		End
		Part1	:WolfSpider_Glow1.part
	End
End


#############################################################

End