#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On		Time
	Time		0

	Event
		Type	Start
		At	Root
		Part	:FrostCircle_Immediate.part
		Part	:Mist_Radial.part
		Part	:Sparkles_Radial.part
	End
End

#########################################################

End