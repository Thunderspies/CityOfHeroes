#########################################################
##	IceBolt
############################
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Root
End


#LifeSpan	200

##################################
Condition
	On 	Time
	Time 	17

	Event
		EName	Hookup
		Type	local
		At	root
		
	End

	Event
		EName 	Prime
		Type	Start 
		At	Origin
		Bhvr	behaviors/IceRain.bhvr
		Geom	FX_IceBlast
		Part1	:icebolttail3B.part
		Part2	:icebolttail2B.part
		Part3	:IceBoltSnowtrail2.part
		Part4	:IceBoltMistTrail2.part
		#Part5	:icebolttaila.part
		
		LifeSpan	21

	End
	
End



End				