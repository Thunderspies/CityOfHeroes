#############################################################
## FifthCol_Portal_Exit.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At 	Chest
		Part	:Oval_Warping.part
		Part	:Oval_Shrinking_Warping.part
		Part	:Oval_Thin_WarpingOut.part
		Lifespan 50
	End

	Event
		Type	Start
		At	Chest
		Part	:Core_Tendrils_Warping.part
		Part	:Oval_Fill_Alpha_Warping.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Chest
		Part	:Embers_Returning.part
		PMagnet	Chest
		Lifespan 35
	End

	Event
		Ename	Prime
		Type	Local
		At	WepR
		Part	:Embers_Returning.part
		POther	UArmR
		PMagnet	Chest
	End

	Event
		Ename	Prime1
		Type	Local
		At	WepL
		Part	:Embers_Returning.part
		POther	UArmL
		PMagnet	Chest
	End

	Event
		Ename	Prime2
		Type	Local
		At	FootR
		Part	:Embers_Returning.part
		POther	ULegR
		PMagnet	Chest
	End

	Event
		Ename	Prime3
		Type	Local
		At	WepL
		Part	:Embers_Returning.part
		POther	ULegL
		PMagnet	Chest
	End
End

Condition
	On	Time
	Time 	30

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

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Prime
		Type	Local
		At	Origin
		ChildFX	GENERIC\entityVanish.fx
		Lifespan 60
	End
End

Condition
	On	Time
	Time	60

	Event
		EName 	Prime
		Type	Local
		At	Origin
		Bhvr	:WarpIn_Fade.bhvr
		Flags	AdoptParentEntity
	End
End

Condition
	On	Time
	Time	50

	Event
		Type	Local
		At	Root
		ChildFX	:BodyGlow_Long.fx
	End
End

#############################################################

End