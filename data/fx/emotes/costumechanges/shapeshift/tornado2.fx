#############################################################
## Tornado.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At 	Origin
		Sound	Emote_Spin_01 80 60 0.8
	End
End

#############################################################

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	InitialGales
		Type	Local
		At	Root
		bhvr    behaviors\SSSpin1.bhvr
		ChildFX	:GustTrail.fx
		Lifespan 20
	End

	Event
		Type	Local
		At	Root
		bhvr    behaviors\SSSpin3.bhvr
          	Part	:Ensnare.part
		Part	:Ensnare2.part
        	Part  	:EnsnareMist2.part
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	InnerTornado
		Type	Local
		At	Root
		ChildFX :InnerTornado.fx
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	40

	Event
		EName 	FogRing1
		Type	Local
		At	Root
		bhvr    behaviors\SSSpin6.bhvr
		BhvrOverride
			PositionOffset 0.0 4.0 0.0
		End
		Part	:LargeBlast2.part
		Part	:LowerBlast3.part
#		ChildFX	:GustTrailOut.fx
		Lifespan 20
	End
End

#############################################################

End

