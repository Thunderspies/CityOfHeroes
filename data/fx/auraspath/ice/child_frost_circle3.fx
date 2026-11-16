#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE

	Event
		Type	Local
		At	Root
		WHILE	MALE
		Part	:FrostCircle.part
		Part	:FrostCircle_Immediate.part
		Part	:Mist_Radial.part
		Part	:Sparkles_Radial.part
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE
	#DoMany		1
	Event
		Type	Local
		At	Root
		WHILE	FEMALE
		Part	:FrostCircle.part
		Part	:FrostCircle_Immediate.part
		Part	:Mist_Radial.part
		Part	:Sparkles_Radial.part
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE
	#DoMany		1
	Event
		Type	Local
		At	Root
		WHILE	HUGE
		Part	:FrostCircle.part
		Part	:FrostCircle_Immediate.part
		Part	:Mist_Radial.part
		Part	:Sparkles_Radial.part
	End
End



#########################################################

End