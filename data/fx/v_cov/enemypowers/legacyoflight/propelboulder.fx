#########################################################
##	Fireweapon
########################################################
FxInfo

Lifespan 300

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
		Type	Posit
		At	Prop_chunk

		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05

		Bhvr	behaviors/GCPropelScale.bhvr
		#Part	:GCRockGlow.part
	End

End


Condition
	On	Time
	Time	36

	Event
		EName	Prop_chunk_FX
		Type	Start
		At	Prop_chunk_node
		  #AnimPiv Hips
#		Part	:GCVortexGround.part
#		Part	:GCGlowGroundSmall.part
#		Part	:GCVortexGlowsUp2.part
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
		EName	Prop_chunk_node2
		Type	Destroy
	End

	Event
		EName	Prop_chunk_node
		Type	Destroy
	End

	Event
		EName 	Prime
		Type	Start
		At	Prop_chunk_node2
		Geom	Parent
		Bhvr	behaviors/GCPropel.bhvr
		Magnet	Target
		LookAt	Target
		Sound Rockthrow 100 100 .9
	End

End

#########################################################
#################### CHUNK EFFECTS ######################
#########################################################

Condition
	On	Time
	Time	90

	Event
		EName	Ring_node
		Type	Local
		At	Prime
		Geom	FrontFacingLocalPivot
		#Lookat	Target
	End
	
	Event	
		ENAME	ChunkRings
		Type	Posit
		At	Ring_node
		AltPiv	1
#		part1	:GCChunkRings.part	
	End

	Event	
		ENAME	RockStreaks
		Type	Posit
		At	Ring_node
#		part	:GCRockStreaks1.part
		Lifespan 60
	
	End
End

Condition
	On	Primehit
	Time	0

	Event
		EName	All
		Type	Destroy
	End

End

#########################################################
#################### BODY GLOW 1 ########################
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

Condition
	On	Time
	Time	10
	
	Event	
		ENAME	NeckNode
		Type	Local
		At	Neck
	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
#		part1	:GCBodyGlow.part
		Pmagnet NeckNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	ChestNode
		Type	Local
		At	Chest
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
#		part1	:GCBodyGlow.part
		Pmagnet ChestNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LarmLNode
		Type	Local
		At	LarmL
	
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
#		part1	:GCBodyGlow.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
#		part1	:GCBodyGlow.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End

End

#########################################################
################## BODY GLOW MIST #######################
#########################################################


	
Condition
	On	Time
	Time	0
	
	Event	
		ENAME	NeckNode
		Type	Local
		At	Neck
	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
#		part1	:GCBodyGlowMist.part
		Pmagnet NeckNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	ChestNode
		Type	Local
		At	Chest
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
#		part1	:GCBodyGlowMist.part
		Pmagnet ChestNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End
		

	Event	
		ENAME	LarmLNode
		Type	Local
		At	LarmL
	
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
#		part1	:GCBodyGlowMist.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
#		part1	:GCBodyGlowMist.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End

End


End
##################################

