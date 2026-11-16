#############################################################
## Alignment_Justice.fx
#############################################################

FxInfo
Lifespan	900

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Local
		At	Chest
		Part	:Glow_Ring_Quick.part

		Lifespan	5
	End

	Event
		EName	Ring
		Type	Local
		At	Chest
		part	:Glow_Large.part
		Part	:Glow_Ring.part
		Part	:Glow_Large_Highlight.part
		Part	:Glow_Large_Highlight_Continuing.part
		Part	:Glow_Large_Continuing.part
	End

	Event
		EName	Ring
		Type	Local
		At	Chest
		part	:LightRay_Up.part
		part	:LightRay_Down.part
		part	:LightRay_Right.part
		part	:LightRay_Left.part
		part	:LightRay_UpLeft.part
		part	:LightRay_UpRight.part
		part	:LightRay_DownLeft.part
		part	:LightRay_DownRight.part
	End

End