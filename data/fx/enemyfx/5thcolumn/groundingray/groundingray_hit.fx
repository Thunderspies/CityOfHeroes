#############################################################
## GroundingRay_Hit.fx
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename 	Offset
		Type	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 12 0
		End
		Part	:Circle_Fill.part
		Part	:GCFlashLrg.part
		Sound GroundingRayHit 40 40 1
		Lifespan 80
	End

	Event
		Ename 	Offset2
		Type	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 12 0
		End
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Start
		At	Offset2
		Part	:GCVortexInFast.part
		Part	:GCVortexInFast_Highlights.part
		Part	:GCVortexInFast_Alpha.part
		Part	:Wormhole_Opaque.part
		Part	:Wormhole_Additive.part
		Part	:Steam_BG.part
		Part	:Embers_Gathering.part
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Root
		ChildFX	:BodyGlow.fx
	End

	Event
		Ename	Prime
		Type	Local
		At	WepR
		Part	:PowerDrain.part
		Part	:PowerDrain_Core.part
		Part	:Embers_Warping.part
		POther	Offset2
		PMagnet	Offset2
		Lifespan 90
	End

	Event
		Ename	Prime1
		Type	Local
		At	WepL
		Part	:PowerDrain.part
		Part	:PowerDrain_Core.part
		Part	:Embers_Warping.part
		POther	Offset2
		PMagnet	Offset2
		Lifespan 90

	End

	Event
		Ename	Prime3
		Type	Local
		At	Hips
		Part	:PowerDrain.part
		Part	:PowerDrain_Core.part
		Part	:Embers_Warping.part
		POther	Offset2
		PMagnet	Offset2
		Lifespan 90
	End
End

##############################################################

End