#########################################################
##	
##

FxInfo


LifeSpan	200

##################################

COndition
	On	Time
	Time	30	#0

	Event
		Type	Local
		At	Origin
		Sound Effigy5 100 100 .75
	End
End


Condition
	On 	Time
	Time 	64	#34

	Event
		Type	local
		At	WepL
		
		Bhvr	:DryHeave.bhvr

		Part	:InitialCloud.part
		Part	:InitialCloud2.part
		Part	:InitialChunks.part

			
		LifeSpan 7
	End

	Event
		Type	local
		At	WepL
		
		Bhvr	:DryHeave.bhvr


		Part	:PoisonSpitBubbles.part
		Part	:PoisonSpitBlack.part
		Part	:PoisonSpit.part
		Part	:DryHeaveStreamDust01.part
		Part	:DryHeaveStreamDust02.part
		Part	:PoisonBallz.part
		Part	:PoisonSpitBlack.part
		Part	:PoisonSpecks.part
		
		Part	:DryHeaveMainStreamChunks01.part
			
		LifeSpan 10
	End

End

Condition
	On 	Time
	Time 	64	#34

	Event
		Type	local
		At	WepL
		
		Bhvr	:DryHeave2.bhvr

		Part	:InitialCloud.part
		Part	:InitialCloud2.part
		Part	:InitialChunks.part

		LifeSpan 14
	End

	Event
		Type	local
		At	WepL
		
		Bhvr	:DryHeave2.bhvr

		Part	:DryHeaveStreamDust01.part
		Part	:DryHeaveStreamDust02.part
		Part	:PoisonBallz.part		
		Part	:PoisonSpecks.part
		Part	:PoisonSpit.part
		Part	:PoisonSpitBubbles.part
		Part	:PoisonSpecksSpit.part
		Part	:PoisonSpecks.part
		
		Part	:DryHeaveMainStreamChunks01.part
		
		LifeSpan 17
	End

End

Condition
	On 	Time
	Time 	64	#34

	Event
		Type	local
		At	WepL
		
		Bhvr	:DryHeave3.bhvr

		Part	:InitialCloud.part
		Part	:InitialCloud2.part
		Part	:InitialChunks.part

		
		LifeSpan 14
	End

	Event
		Type	local
		At	WepL
		
		Bhvr	:DryHeave3.bhvr


		Part	:DryHeaveStreamDust01.part
		Part	:DryHeaveStreamDust02.part
		Part	:PoisonBallz.part		
		Part	:PoisonSpecks.part
		Part	:PoisonSpit.part
		Part	:PoisonSpitBubbles.part
		Part	:PoisonSpecksSpit.part
		Part	:PoisonSpecks.part
		
		Part	:DryHeaveMainStreamChunks01.part
		
		LifeSpan 17
	End

End


Condition
	On 	Time
	Time 	75	#45
	
	Event	
		ENAME	ChunkLine
		Type	local
		At	T_Root

#		Bhvr	:DryHeave.bhvr

		part	:DryHeaveBeetleLine01.part
		part	:DryHeaveChunkLine01.part
		part	:DryHeaveSplatter.part
		part	:DryHeaveSplatterMist.part
		
		Lifespan 85
	
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

#
#Condition
#	On 	PrimeHit
#
#	Event
#		EName	DustHit
#		Type	Start
#		At	Target
#		Part	:DryHeaveBeetleBlast01.part
#		Part	:DryHeaveChunkBlast01.part
#		Part	:DryHeaveHitDust01.part
#		LifeSpan 10
#	End
#
#	Event
#		EName	DustHit
#		Type	Start
#		At	T_Root
#		Part	:DryHeaveHitDustRing01.part 
#		bhvr	:RingScale.bhvr
#		LifeSpan 40
#	End
#
#	Event	
#		ENAME	Ring
#		Type	Start
#		At	T_Root
#		part	:DryHeaveFlatRing01.part
#		part	:DryHeaveBeetleRing01.part
#		part	:DryHeaveChunks01.part
#		bhvr	:Adamastor.bhvr
#		Lifespan 10
#	End
#
#End
#

End

	