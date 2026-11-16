FxInfo






#########################################################
################# INITAL WIND TRAILS ####################
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	InitialGales
		Type	Local
		At	Root
	#	Pmagnet Chest
		bhvr    behaviors/SSSpin1.bhvr
		ChildFX	:GustTrail.fx
		Lifespan 45
	End

   	Event
                EName   Prime
                Type    local
                At      Root
                Geom    Dummy02
        End

End

Condition
	On 	Time
	Time 	10

	Event
		EName	InnerTornado
		Type	Local
		At	Root
		ChildFX :InnerTornado.fx
		Lifespan 40

	End

	Event
		EName	Debris
		Type	Local
		At	Root
		bhvr    behaviors/SSSpin3.bhvr
                Part   :Ensnare.part
                Part   :EnsnareMist2.part
		Lifespan 50
	End
End

Condition
	On 	Time
	Time 	50

	Event
		EName 	FogRing1
		Type	Local
		At	Root
		AltPiv	1
		bhvr    behaviors/SSSpin6.bhvr
		BhvrOverride
			PositionOffset 0.0 4.0 0.0
		End
		Part	:LargeBlast2.part
		Part	:LowerBlast3.part
		ChildFX	:GustTrailOut.fx
		Lifespan 20

	End

End
#Condition
#        On      Cycle
#        Time    5
#        Chance  1
#
#
#   	Event
#                EName   Prime
#                Type    local
#                At      Root
#                Geom    Dummy02
#        End
#
#        Event
#                EName   Rings
#                Type    Local
#                At      Prime
#
#                Bhvr    behaviors/slowtornadomotion.bhvr
#		BhvrOverride
#			StartColor 0 0 255
#		End
#                geom    SpeedWisp       #PsionicWisp
#
#                Lifespan 30
#        End

End

