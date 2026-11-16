#############################################################
## StoneHands.fx
#############################################################

FxInfo

Lifespan 36

Flags DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\stonehands.bhvr
		geom	StoneHandR
		Sound stonepunch 80 80 0.8
	End

	Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\stonehands.bhvr
		geom	StoneHandL
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	LARMR
		Part	:EarthHandsDust.part
		Part	:EarthHandsDust2.part
		Part	:EarthHandsDustRocks.part
	End

	Event
		Type	Local
		At	LARML
		Part	:EarthHandsDust.part
		Part	:EarthHandsDust2.part
		Part	:EarthHandsDustRocks.part
	End
End

#############################################################

End