#########################################################
##	Fireweapon
########################################################
FxInfo

Lifespan 300

#########################################################
#################### Sound ########################
#########################################################


Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound stoneweaponpickup4 80 80 1.0
	End
End

##################################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Prop_rise
		Type	Start
		At	Root 
		Anim	Prop_basic
		Sound thunder2 100 100 .66
	End

End

Condition
	On	Time
	Time	19

	Event
		EName	Prop_chunk
		Type	Local
		At	Prop_rise
		  AnimPiv Hips
	End

	Event
		EName	Prop_chunk_node
		Type	Posit
		At	Prop_chunk
		#Sound	gravityzap4 100 30 .8
	End


End

Condition
	On	Time
	Time	40

	Event
		EName	Prop_chunk_node2
		Type	Posit
		At	Prop_chunk

		part	:EarthEmbraceDust.part
		part	:EarthEmbraceDust2.part
		part	:EarthEmbraceRocks01.part
		part	:EarthEmbraceRocks02.part
		
		LifeSpan	40
	
	End

	Event
		EName	Prop_chunk_node2
		Type	local
		At	Prop_chunk

		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05

		Bhvr	behaviors/RockPropelScale.bhvr
		#Part	:GCRockGlow.part
	End

End

###########################################################################################

Condition
	On	Time
	Time	43

	Event
		EName	Prop_chunk_node1
		Type	local
		At	Prop_chunk

		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05

		Bhvr	behaviors/RockPropelScale2.bhvr
		#Part	:GCRockGlow.part
	End
End

Condition
	On	Time
	Time	47

	Event
		EName	Prop_chunk_node3
		Type	local
		At	Prop_chunk

		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05

		Bhvr	behaviors/RockPropelScale3.bhvr
		#Part	:GCRockGlow.part
	End
End

Condition
	On	Time
	Time	50

	Event
		EName	Prop_chunk_node4
		Type	local
		At	Prop_chunk

		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05

		Bhvr	behaviors/RockPropelScale4.bhvr
		#Part	:GCRockGlow.part
	End
End

Condition
	On	Time
	Time	52

	Event
		EName	Prop_chunk_node5
		Type	local
		At	Prop_chunk

		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05

		Bhvr	behaviors/RockPropelScale5.bhvr
		#Part	:GCRockGlow.part
	End
End

Condition
	On 	Time
	Time 	90

	Event
		EName	Prop_chunk
		Type	Destroy
	End

	Event
		EName	Prop_chunk_node1
		Type	Destroy
	End

	Event
		EName	Prop_chunk_node2
		Type	Destroy
	End

	Event
		EName	Prop_chunk_node3
		Type	Destroy
	End

	Event
		EName	Prop_chunk_node4
		Type	Destroy
	End

	Event
		EName	Prop_chunk_node5
		Type	Destroy
	End

	Event
		EName	Prop_chunk_node
		Type	Destroy
	End

	Event
		EName 	Prime
		Type	Start
		At	Prop_chunk_node1
		Geom	Parent
		Bhvr	behaviors/RockPropel.bhvr
		Magnet	Target
		LookAt	Target
		Sound Rockthrow 100 100 .9
	End

	Event
		EName 	Prime1
		Type	Start
		At	Prop_chunk_node2
		Geom	Parent
		Bhvr	behaviors/RockPropel1.bhvr
		Magnet	T_LArmR
		LookAt	Target
		Sound Rockthrow 100 100 .9
	End
	
	Event
		EName 	Prime2
		Type	Start
		At	Prop_chunk_node3
		Geom	Parent
		Bhvr	behaviors/RockPropel2.bhvr
		Magnet	T_UlegL
		LookAt	Target
		Sound Rockthrow 100 100 .9
	End

	Event
		EName 	Prime3
		Type	Start
		At	Prop_chunk_node2
		Geom	Parent
		Bhvr	behaviors/RockPropel3.bhvr
		Magnet	T_llegR
		LookAt	Target
		Sound Rockthrow 100 100 .9
	End
	
	Event
		EName 	Prime4
		Type	Start
		At	Prop_chunk_node3
		Geom	Parent
		Bhvr	behaviors/RockPropel4.bhvr
		Magnet	T_Head
		LookAt	Target
		Sound Rockthrow 100 100 .9
	End

End

Condition
	On	Primehit

	Event
		EName	Prime
		Type	Destroy
	End

End

Condition
	On	Prime1hit

	Event
		EName	Prime1
		Type	Destroy
	End

End

Condition
	On	Prime2hit

	Event
		EName	Prime2
		Type	Destroy
	End

End

Condition
	On	Prime3hit

	Event
		EName	Prime3
		Type	Destroy
	End

End

Condition
	On	Prime4hit

	Event
		EName	Prime4
		Type	Destroy
	End

End

End
##################################

