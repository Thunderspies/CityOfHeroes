#############################################################
## Tornado.fx
#############################################################

FxInfo
Lifespan 95

#############################################################
Condition
	On 	Time
	Time 	10

	Event
		EName	GalesAnchor
		Type	Start
		At	Root
		Lifespan 70
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Root
		bhvr    behaviors\SSSpin1.bhvr
		ChildFX	:GustTrail.fx
		Lifespan 55
	End

	Event
		Type	Local
		At	Root
		bhvr    behaviors\SSSpin3.bhvr
		Part	:Ensnare2.part
        	Part  	:EnsnareMist2.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	TornadoAnchor
		Type	Start
		At	Root
		Lifespan 35
	End

	Event
		Type	Local
		At	Root
		bhvr    behaviors\SSSpin3.bhvr
		BhvrOverride
			Spin	0.000 -0.35 0.000000
		End
		Part	:CirclingDust_Opaque.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	45

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			FadeOutLength		15
		End

		Flags	AdoptParentEntity
		Lifespan 1
	End
End

#Condition
#	On	Time
#	Time	60
#
#	Event
#		Type	Local
#		At	Root
#		Bhvr	behaviors\fadeIN.bhvr
#		BhvrOverride
#			FadeInLength		15
#		End
#
#		Flags	AdoptParentEntity
#		Lifespan 1
#	End
#End
#############################################################

Condition
	On	Cycle
	Time	10

        Event
                EName   Rings
                Type    Local
                At      TornadoAnchor
                Bhvr   behaviors\tornado.bhvr #behaviors\Tornado_CostumeChange.bhvr
		BhvrOverride
			Alpha		90
			StartColor	255 255 255
			Scale		0.25 1.0 0.25
			ScaleRate	1.012 1.018 1.012
 			EndScale	1.5 0.9 1.5

			Spin		0 1.0 0
			SpinJitter	0 0.5 0
			Gravity		-.0125
		End
           	geom   SpeedWisp_Grayscale
		Lifespan 30
        End
End

Condition
	On 	Cycle
	Time 	10
	Repeat	2

	Event
		EName	InnerTornado
		Type	Local
		At	GalesAnchor
		ChildFX :OuterTornado.fx
		Lifespan 70
	End
End

#############################################################
##################   PHYSICS  ###############################
#############################################################

Condition

	On	Time
	Time 	40

	Event
		Ename	PHYSICS-ExplosiveForce
		Type	Start
		At	Chest

		BhvrOverride
			PhysForceType		CWSwirl
			PhysForceRadius		15
			PhysForcePower		400
			PhysForcePowerJitter	75
		End
		Lifespan	25

	End
End
#############################################################

End

