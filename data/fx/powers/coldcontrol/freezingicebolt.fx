#########################################################
##	IceBolt
############################
FxInfo

Input  
	InpName	Origin
End

LifeSpan	21

##################################
Condition
	On 	Time
	Time 	0

	Event
		EName	Hookup
		Type	local
		At	Origin
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName 	Prime
		Type	Start 
		At	Hookup
		Bhvr	behaviors/IceRain.bhvr
		Geom	FX_IceBlast
		Part1	:icebolttail3a.part
		Part2	:icebolttail2a.part
		Part3	:IceBoltSnowtrail2.part
		Part4	:IceBoltMistTrail2.part
		#Part5	:icebolttaila.part
		
	End
	
End



End				