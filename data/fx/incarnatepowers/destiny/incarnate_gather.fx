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
	On	Time
	Time	0

	Event
		Type	start
		At	Root
		Sound explohit 80 80 0.77
	End
End

## POWER FX #######################################################


Condition
	On	Time
	Time	1

	Event
		EName	AOERing
		Type	start
		At	Root
		BhvrOverride
          		PositionOffset 0.0 5 0.0
         	End
		Part2	:Incarnate_GatherDust.part
		Lifespan	10
	End

End
#Condition
#	On	Time
#	Time	10
#
#	Event
#		EName	AOERing
#		Type	start
#		At	Root
#		BhvrOverride
#          		PositionOffset 0.0 0.5 0.0
#         	End
#		Part2	:Incarnate_BarrierFloorB.part
#		Lifespan	10
#	End
#
#End
#Condition
#	On	Time
#	Time	46
#
#	Event
#		EName	AOERing
#		Type	start
#		At	Root
#		BhvrOverride
#          		PositionOffset 0.0 .5 0.0
#         	End
#		Part2	:Incarnate_BarrierFloorC.part
#		Lifespan	10
#	End
#
#End
#Condition
#	On	Time
#	Time	55
#
#	Event
#		EName	AOERing
#		Type	start
#		At	Root
#
#     		BhvrOverride
#          		PositionOffset 0.0 5 0.0
#         		Gravity 0
#     		End
#     		Part2	:Incarnate_BarrierGlow.part
#		Lifespan	10
#	End
#
#End
##Condition
##	On	Time
##	Time	1
##
##	Event
##		EName	AOERing
##		Type	start
##		At	Root
##		Part2	:Incarnate_BarrierShield.part
##		Lifespan	10
##	End
##
##End
##Condition
##	On	Time
##	Time	1
##	Event
##		EName	AOERing
##		Type	start
##		At	Root
##		Part2	:Test_EarthCrack2.part
##		Lifespan	1
##	End
##
##End
#
#
#
#Condition
#	On	Time
#	Time	10
#	Event
#		Ename 	RockANchor
#		Type	Start
#		At	Root
#		Bhvr	:Incarnate_BarrierSpherePart.bhvr
#		BhvrOverride
#
##			Scale			0.1  0.1  0.1
##			ScaleRate		1.11 1.11 1.11
##			EndScale		1.2 1.2 1.2  #1.1 1.1 1.1
##			Scale			0.25 0.25 0.25
##			ScaleRate		1 1 1
##			endScale		3 3 3
#			StartColor 		255 255 255
#			BeColor1 		0 128 255
#
#			ByTime1 8
#			BeColor2 		255 255 255
#
#			ByTime1 16
#			Alpha			64
#			FadeOutLength		10
#			PyrRotate 		0.0 0.0 0.0
#			PyrRotateJitter 	0.0 0.0 0.0
#		End
#
#
##		Geom	Incarnate_Barrier_ShieldPart
#		Geom	Incarnate_Barrier_ShieldA
#		Lifespan 2
#		LifespanJitter	0
#		CThresh 	0.00001
#	End
#End
##Condition
##	On	Time
##	Time	20
##
##	Event
##		Ename 	RockANchor
##		Type	Start
##		At	Root
##		Bhvr	:Incarnate_BarrierSpherePartB.bhvr
##		BhvrOverride
##
###			Scale			0.1  0.1  0.1
###			ScaleRate		1.11 1.11 1.11
###			EndScale		1.2 1.2 1.2  #1.1 1.1 1.1
###			StartColor 		0 128 255
##			Alpha 			32
##			FadeOutLength		15
##			PyrRotate 		0.0 0.0 0.0
##			PyrRotateJitter 	0.0 0.0 0.0
##		End
##
##
###		Geom	Incarnate_Barrier_ShieldPart
##		Geom	Incarnate_Barrier_ShieldA
##		Lifespan 1
##		LifespanJitter	0
##		CThresh 	0.00001
##	End
##End
##Condition
##	On	Time
##	Time	25
##
##	Event
##		Ename 	RockANchor
##		Type	Start
##		At	Root
##		Bhvr	:Incarnate_BarrierSpherePartC.bhvr
##		BhvrOverride
##
###			Scale			0.1  0.1  0.1
###			ScaleRate		1.11 1.11 1.11
###			EndScale		1.2 1.2 1.2  #1.1 1.1 1.1
###			StartColor 		0 128 255
##			Alpha			64
##			FadeOutLength		25
##			PyrRotate 		0.0 0.0 0.0
##			PyrRotateJitter 	0.0 0.0 0.0
##		End
##
##
###		Geom	Incarnate_Barrier_ShieldPart
##		Geom	Incarnate_Barrier_ShieldA
##		Lifespan 5
##		LifespanJitter	0
##		CThresh 	0.00001
##	End
##End
#
#
##---------------------------------------
##Condition
##	On	Time
##	Time	40
##
##	Event
##		Ename 	RockANchor
##		Type	Start
##		At	Root
##		Bhvr	:Incarnate_BarrierSpherePartC.bhvr
##		BhvrOverride
##
###			Scale			0.1  0.1  0.1
###			ScaleRate		1.11 1.11 1.11
###			EndScale		1.2 1.2 1.2  #1.1 1.1 1.1
##			Alpha			160
##			FadeOutLength		25
##			PyrRotate 		0.0 0.0 0.0
##			PyrRotateJitter 	0.0 0.0 0.0
##		End
##
##
###		Geom	Incarnate_Barrier_ShieldPartB
##		Geom	Incarnate_Barrier_ShieldA
##		Lifespan 1
##		LifespanJitter	0
##		CThresh 	0.00001
##	End
##End
#Condition
#	On	Time
#	Time	38
#	Event
#		Ename 	RockANchor
#		Type	Start
#		At	Root
#		Bhvr	:Incarnate_BarrierSpherehalf.bhvr
#		BhvrOverride
#
##			Scale			0.1  0.1  0.1
##			ScaleRate		1.11 1.11 1.11
##			EndScale		1.2 1.2 1.2  #1.1 1.1 1.1
#
#			FadeOutLength		25
#			PyrRotate 		0.0 0.0 0.0
#			PyrRotateJitter 	0.0 0.0 0.0
#		End
#
#
#		Geom	Incarnate_Barrier_ShieldHalfB
#
#		Lifespan 5
#		LifespanJitter	0
#		CThresh 	0.00001
#	End
#End
#
#Condition
#	On	Time
#	Time	45
#	Event
#		Ename 	RockANchor
#		Type	Start
#		At	Root
#		Bhvr	:Incarnate_BarrierSpherehalf.bhvr
#		BhvrOverride
#
##			Scale			0.1  0.1  0.1
##			ScaleRate		1.11 1.11 1.11
##			EndScale		1.2 1.2 1.2  #1.1 1.1 1.1
#			StartColor 		128 255 255
##			BeColor1 		0   128 255
##
##			ByTime1 8
#
#
#			Alpha			255
#			FadeOutLength		25
#			PyrRotate 		0.0 0.0 0.0
#			PyrRotateJitter 	0.0 0.0 0.0
#		End
#
#
#		Geom	Incarnate_Barrier_ShieldHalf
#
#		Lifespan 5
#		LifespanJitter	0
#		CThresh 	0.00001
#	End
#End
#
#Condition
#	On	Time
#	Time	55
#	Event
#		Ename 	RockANchor
#		Type	Start
#		At	Root
#		Bhvr	:Incarnate_BarrierSphere.bhvr
#		BhvrOverride
#
##			Scale			0.1  0.1  0.1
##			ScaleRate		1.11 1.11 1.11
##			EndScale		1.2 1.2 1.2  #1.1 1.1 1.1
#			FadeInLength		6
#			Alpha			64
#			FadeOutLength		25
#			PyrRotate 		0.0 0.0 0.0
#			PyrRotateJitter 	0.0 0.0 0.0
#		End
#
#
#		Geom	Incarnate_Barrier_Shield
#
#		Lifespan 200
#		LifespanJitter	0
#		CThresh 	0.00001
#	End
#End
##########################################################

End