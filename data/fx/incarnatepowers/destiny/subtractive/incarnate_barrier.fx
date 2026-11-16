#########################################################
## IncarnatePower  - Barrier
#########################################################

FxInfo
LifeSpan 300

########################################################

Input
	Inpname	Hips
End

########################################################


## SOUND FX #######################################################

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Barrier_01 100 100 .8
	End
End

## POWER FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		ChildFX	IncarnatePowers\Incarnate_Gather.fx
		Lifespan	30
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	AOERing
		Type	start
		At	Root
		BhvrOverride
          		PositionOffset 0.0 0 0.0
         	End
		Part2	:Incarnate_BarrierFloorA.part
		Lifespan	20
	End

End

Condition
	On	Time
	Time	20
	Event
		Ename 	RockANchor
		Type	PositOnly
		At	HandR
		Bhvr	:Incarnate_BarrierSpherehand.bhvr
		BhvrOverride
			PyrRotateJitter 	0.0 90 0.0
			TintGeom 		1
		End
		Geom	Incarnate_Barrier_Shieldhand_Dark
		Part2	:Incarnate_BarrierHandGlow.part
		Lifespan 35
		LifespanJitter	0
		CThresh 	0.00001
	End

	Event
		Ename 	RockANchor
		Type	PositOnly
		At	HandL
		Bhvr	:Incarnate_BarrierSpherehand.bhvr
		BhvrOverride
			TintGeom 		1
		End
		Geom	Incarnate_Barrier_ShieldHand_Dark
		Part2	:Incarnate_BarrierHandGlow.part
		Lifespan 35
		LifespanJitter	0
	End
End

Condition
	On	Time
	Time	60

	Event
		EName	AOERing
		Type	start
		At	Root
     		Part2	:Incarnate_BarrierBam.part
		Lifespan	3
	End

End

Condition
	On	Time
	Time	60

	Event
		EName	AOERing
		Type	start
		At	Root

     		BhvrOverride
          		PositionOffset 0.0 3 0.0
     		End
     		Part2	:Incarnate_BarrierLightning.part
		Lifespan	1
	End

End

Condition
	On	Time
	Time	60

	Event
		EName	AOERing
		Type	start
		At	Root
     		Part2	:Incarnate_BarrierLightningRing.part
		Part2	:Incarnate_BarrierLightningRingGround.part
		Lifespan	70
	End

End


Condition
	On	Time
	Time	58
	Event
		Ename 	RockANchor
		Type	Start
		At	Root
		Bhvr	:Incarnate_BarrierSpherePart.bhvr
		BhvrOverride
			TintGeom 1
		End
		Geom	Incarnate_Barrier_ShieldA_Dark
		Lifespan 10
		LifespanJitter	0
		CThresh 	0.00001
	End
End

Condition
	On	Time
	Time	60
	Event
		Ename 	RockANchor
		Type	Start
		At	Root
		Bhvr	:Incarnate_BarrierSphere.bhvr
		BhvrOverride


			FadeInLength		6
			Alpha			32
			FadeOutLength		25
			TintGeom 		1
		End

		Geom	Incarnate_Barrier_ShieldA_Dark

		Lifespan 60
		LifespanJitter	0
		CThresh 	0.00001
	End
End
#########################################################

End