#########################################################
##	
##

FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Root
End

Input  
	InpName	Head
End

Input  
	InpName	T_Root
End




LifeSpan	200

##################################

COndition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound Effigy5 100 100 .75
	End
End


Condition
	On 	Time
	Time 	56

	Event
		EName	Prime
		Type	Start
		At	Head
		#Part	:DryHeaveMainStreamChunks01.part
		Part	:DryHeaveStreamDust02.part
		bhvr	:DryHeave.bhvr
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On 	Time
	Time 	60

	Event
		EName	Chunks
		Type	Start
		At	Head
		Part	:DryHeaveMainStreamChunks01.part
		#Part	:DryHeaveStreamDust02.part
		bhvr	:DryHeave.bhvr
		Magnet	Target
		LookAt	Target
	End
End
Condition
	On 	Time
	Time 	60

	Event
		EName	FallingRoot
		Type	Start
		At	Root
		#Geom	FX_ICEWEAPON
		#Bhvr	:adamastor.bhvr
	End

	Event
		EName	FallingTRoot
		Type	Start
		At	T_Root
		#Geom	FX_ICEWEAPON
		Bhvr	:adamastor.bhvr
	End
End

Condition
	On 	Time
	Time 	78

	Event	
		ENAME	ChunkLine
		Type	Start
		At	FallingTRoot
		part	:DryHeaveBeetleLine01.part
		part	:DryHeaveChunkLine01.part
		POther	FallingRoot
		Lifespan 15
	
	End
End

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End

	Event
		Ename 	Chunks
		Type	Destroy
	End
End


Condition
	On 	Time
	Time 	34

	Event
		EName	DustNode
		Type	Local
		At	Head
		Geom	FX_HuskMouthPivot
	End

	Event
		EName	Dust
		Type	Local
		At	DustNode
		AltPiv	1
		Part	:DryHeaveStreamDust01.part
		PMagnet Target
		POther	Target
		LifeSpan 50
	End
End



Condition
	On 	PrimeHit

	Event
		EName	DustHit
		Type	Start
		At	Target
		Part	:DryHeaveBeetleBlast01.part
		Part	:DryHeaveChunkBlast01.part
		Part	:DryHeaveHitDust01.part
		LifeSpan 10
	End

	Event
		EName	DustHit
		Type	Start
		At	T_Root
		Part	:DryHeaveHitDustRing01.part 
		bhvr	:RingScale.bhvr
		LifeSpan 40
	End

	Event	
		ENAME	Ring
		Type	Start
		At	T_Root
		#Geom	FX_ICEWEAPON
		part	:DryHeaveFlatRing01.part
		part	:DryHeaveBeetleRing01.part
		part	:DryHeaveChunks01.part
		bhvr	:Adamastor.bhvr
		Lifespan 10
	End

End


End

	