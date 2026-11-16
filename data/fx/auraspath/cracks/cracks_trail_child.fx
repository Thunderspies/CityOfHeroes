#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0
	#Domany	1

	Event
		Ename	Crackin
		Type	PositOnly
		At	Root
		UNTIL	FLY
		flags	OneAtATime
		Bhvr	Behaviors/genericParticleFade.bhvr
		Part	:Cracks02.part
		Part	:Cracks03.part
		Part	:Cracks04.part
		CHILDFX	:CracksTrail_Dust.fx
	end
End



#########################################################

End