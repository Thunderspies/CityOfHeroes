#########################################################
##	IceBolt
############################
FxInfo


LifeSpan	200

##################################

Condition
	On	Time
	Time	5

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:snowburst.part
		
		LifeSpan	37
	End
		



	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		
		Part2	:ColdArmMist.part

		LifeSpan	37
	End
		


End

Condition
	On 	Time
	Time 	15.2

	Event
		EName 	Prime
		Type	Start 
		At	WepL
		Bhvr	behaviors/powers/coldcontrol/iceblast.bhvr
		Geom	FX_IceBlast
		Magnet	Target
		LookAt	Target
		Part1	:icebolttail3.part
		Part2	:icebolttail2.part
		Part3	:IceBoltSnowtrail2.part
		Part4	:IceBoltMistTrail2.part
		Part5	:icebolttail.part
		
	End
	
End
###########################
Condition
	On 	Time
	Time 	0



	Event
		EName 	fist
		Type	Local 
		At	WepL

		Sound Ice9 100.0 100.0 .75

	End


End


##############################################################


Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

	Event	
		Ename   IceShards
		Type	Posit
		At	target
		Part1	:IceBoltMist.part
		Part2	:ChillTouchSnow.part
		Part3   :ChillingTouchFlash.part

		LifeSpan	20

	End

End


End				