#############################################################
## FifthCol_Portal_Enter.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part	:Embers_Gathering.part
		PMagnet	Chest
		Lifespan 70
	End

	Event
		Type	Local
		At	UArmR
		Part	:GlowQuick2.part
		Part	:ElectricBeam3.part
		PMagnet	Chest
		Lifespan 70
	End

	Event
		Type	Local
		At	UArmR
		Part	:GlowQuick2.part
		Part	:ElectricBeam5.part
		PMagnet	UArmL
		Lifespan 70
	End

	Event
		Type	Local
		At	LArmR
		Part	:ElectricBeam5.part
		PMagnet	UArmR
		Lifespan 70
	End

	Event
		Type	Local
		At	UArmL
		Part	:GlowQuick2.part
		Part	:ElectricBeam3.part
		PMagnet	Chest
		Lifespan 70
	End

	Event
		Type	Local
		At	UArmL
		Part	:GlowQuick2.part
		Part	:ElectricBeam5.part
		PMagnet	UArmR
		Lifespan 70
	End

	Event
		Type	Local
		At	UArmL
		Part	:GlowQuick2.part
		Part	:ElectricBeam3.part
		PMagnet	Chest
		Lifespan 70
	End

	Event
		Type	Local
		At	LArmL
		Part	:ElectricBeam5.part
		PMagnet	UArmL
		Lifespan 70
	End

	Event
		Type	Local
		At	LLegR
		Part	:ElectricBeam3.part
		PMagnet	ULegR
		Lifespan 70
	End

	Event
		Type	Local
		At	LLegL
		Part	:ElectricBeam3.part
		PMagnet	ULegL
		Lifespan 70
	End

	Event
		Type	Local
		At	FootR
		Part	:GlowQuick2.part
		Part	:ElectricBeam4.part
		POther	WepR
		Lifespan 70
	End

	Event
		Type	Local
		At	FootR
		Part	:GlowQuick2.part
		Part	:ElectricBeam4.part
		POther	LLegR
		Lifespan 70
	End

	Event
		Type	Local
		At	FootL
		Part	:GlowQuick2.part
		Part	:ElectricBeam4.part
		POther	WepL
		Lifespan 60
	End

	Event
		Type	Local
		At	FootL
		Part	:GlowQuick2.part
		Part	:ElectricBeam4.part
		POther	LLegL
		Lifespan 70
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	Start
		At	Chest
		Part	:Embers_Gathering.part
		Part	:Core_Tendrils_Warping.part
		PMagnet	Chest
		Lifespan 75
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	Start
		At 	Chest
		Part	:Oval_Warping.part
		Part	:Oval_Shrinking_Warping.part
		Lifespan 70
	End

	Event
		Type	Start
		At	Chest
		Part	:Core_Tendrils_Warping.part
		Part	:Oval_Fill_Alpha_Warping.part
		Lifespan 80
	End

	Event
		Type	Start
		At	Chest
		Part	:Oval_Thin_WarpingOut.part
		Part	:Core_Glow_Warping.part
		Lifespan 95
	End
End

#############################################################

Condition
	On	Time
	Time	75

	Event
		Type	Local
		At	Root
		ChildFX	:BodyGlow.fx
	End

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors\quick_fadeout.bhvr
		Flags	AdoptParentEntity
		Lifespan 1
	End
End

#############################################################

Condition
	On	Time
	Time	80

	Event
		Ename	Prime
		Type	Local
		At	WepR
		Part	:Embers_Warping.part
		POther	UArmR
		PMagnet	Chest
	End

	Event
		Ename	Prime1
		Type	Local
		At	WepL
		Part	:Embers_Warping.part
		POther	UArmL
		PMagnet	Chest
	End

	Event
		Ename	Prime2
		Type	Local
		At	FootR
		Part	:Embers_Warping.part
		POther	ULegR
		PMagnet	Chest
	End

	Event
		Ename	Prime3
		Type	Local
		At	WepL
		Part	:Embers_Warping.part
		POther	ULegL
		PMagnet	Chest
	End
End

#############################################################

End