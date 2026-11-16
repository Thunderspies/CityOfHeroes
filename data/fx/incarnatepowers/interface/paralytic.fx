#############################################################
## Paralytic.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	Chest
		part	:Flash.part
		Lifespan 5
	End

End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Paralytic_Loop 100 100 .3
		bhvr	behaviors/Sound/SoundIn05Out10.bhvr
		lifespan 150
	End

	Event
		Type	Local
		At	Chest
		part	:Glow.part
		part2	:GoldenCenter.part
		part3	:Lingering_Cloud.part
	End

	Event
		ENAME	SpinNode
		Type	Local
		At	Chest
		Bhvr	POWERS\Kinetics\KineticScale.bhvr
		BhvrOverride
			PositionOffset	0 0 0
		End
	End

	Event
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		part	:Kin_Rings.part
		Bhvr	POWERS\Kinetics\KineticSpin1.bhvr
	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	:Kin_Rings.part

		Bhvr	POWERS\Kinetics\KineticSpin2.bhvr
	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	:Kin_Rings.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
	End

	Event
		ENAME	Swirl4
		Type	Local
		At	SpinNode
		part	:Kin_Rings.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		BhvrOverride
			PyrRotate	180 0 0
		End
	End


#	Event
#		ENAME	Swirl3
#		Type	Local
#		At	SpinNode
#		part	:Kin_Rings.part
#		Bhvr	POWERS\Kinetics\KineticSpin4.bhvr
#	End

	Event
		ENAME	Clouds
		Type	PositOnly
		At	SpinNode
		Part	:Lingering_Cloud.part
		BhvrOverride
			PositionOffset	0 0.2 0
		End
	End


#############################################################


End