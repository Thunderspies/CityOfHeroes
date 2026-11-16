#############################################################
## Reveal.fx
#############################################################

FxInfo
LifeSpan	100

############################################################
######	CENTER LIGHT   #####################################
############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Lightray
		Type	Local
		At	Root
		Part	:Lightrays_Center.part
		Lifespan	50
	End

	Event
		Ename	GroundLight
		Type	Local
		At	Root
		Part	:Light_Ground.part
		Lifespan	80
	End
End

############################################################
######	CASCADING RINGS   ##################################
############################################################


Condition
	On	Time
	Time	5


	Event
		EName	RingsDown
		Type	PositOnly
		At	Neck
		Part	:Rings_Cascading.part
		Lifespan 	50
	End
End

############################################################
######	EXPANDING RINGS   ##################################
############################################################

Condition
	On 	Time
	Time 	10




	Event
		EName	RingsOut
		Type	Local
		At	Root
		Part	:Lightrays_Side.part
		Lifespan	60
	End

	Event
		EName	RingsOut
		Type	Local
		At	Root
		Part	:Rings_Solid.part
		Part	:Rings_Clouds.part
		Lifespan	60
	End
End