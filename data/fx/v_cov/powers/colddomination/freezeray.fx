#########################################################
##	IceBolt
############################
FxInfo

LifeSpan	200

###############################################################################
##SOUND
#############################################################

Condition
	On 	Time
	Time 	0



	Event
		EName 	fist
		Type	Local 
		At	WepR

		Sound FreezeRay2 100.0 100.0 .8

	End


End

####################################################################

Condition
	On	Time
	Time	15

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:snowburst.part
		
		LifeSpan	37
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
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
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part2	:ColdArmMist.part

		LifeSpan	37

	End

End
##################################################################

Condition
	On	Time
	Time	15
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:IceChunksHands.part

		LifeSpan	15
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	:MainIceblast3.bhvr
		Geom	FX_IceBlast
		Magnet	Target
		LookAt	Target
		Part1	:FreezeRayGlowA.part
	End
	
	Event
		EName 	Prime2
		Type	Start 
		At	WepR
		Bhvr	:MainIceblast2.bhvr
		Magnet	Target
		LookAt	Target
	End
	
	Event
		EName 	GeometryCrystals1
		Type	local 
		At	Prime
		Magnet	Target
		LookAt	Target
		Bhvr	:iceblast.bhvr
		Geom	ColdBlast1
	End

	Event
		EName 	GeometryCrystals2
		Type	local 
		At	Prime
		Magnet	Target
		LookAt	Target
		Bhvr	:iceblast2.bhvr
		Geom	ColdBlast2
	End

	Event
		EName 	trail
		Type	Start 
		At	Prime
		Magnet	Target
		#LookAt	Target
		Bhvr	:Mainiceblast.bhvr
		Part1	:FreezeRayMistA.part
		Part5	:IceBoltSnowtrail2.part
		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
	End

End

##############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	start 
		At	WepR
		Part1	:IceBoltSnowSkatter.part
		LifeSpan	30
	End

	Event
		Type	start 
		At	WepL
		Part1	:IceBoltSnowSkatter.part
		LifeSpan	30
	End
End

#############################################################################################3



Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End
		
	Event
		Ename	Prime2
		Type	Destroy
	End

	Event
		Ename	trail
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