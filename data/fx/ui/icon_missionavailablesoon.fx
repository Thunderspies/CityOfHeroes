#############################################################
## Icon_MissionInProgress.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Condition
	On 	Time
	Time	 0

	Event
		Ename	GlowAnchor
		Type	Start
		At	Head
		BhvrOverride
			PositionOffset	0 2 0 #3 2 0
		End
		Part	:Star_Red.part
		Part	:Star_Small.part
	End
End

Condition
	On	Cycle
	Time	105
	Event
		Type	Start
		At	GlowAnchor
		Part	:Dial.part
		Lifespan 1
	End
End

#############################################################

End