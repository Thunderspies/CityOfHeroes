#########################################################
##	IceBolt
############################
FxInfo

LifeSpan	400

###############################################################################
##SOUND
#############################################################

Condition
	On 	Time
	Time 	8



	Event
		EName 	fist
		Type	Local 
		At	WepR

		Sound IceNova4 80.0 80.0 .9

	End


End

Condition
	On 	Time
	Time 	22



	Event
		EName 	fist
		Type	Local 
		At	WepR

		#Sound	IceNova 80.0 30.0 1.0

	End


End

####################################################################

Condition
	On	Time
	Time	10

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
	Time	18
		

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
	Time 	27

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	:MainIceblastFast.bhvr
		Geom	FX_IceBlast
		Magnet	Target
		LookAt	Target
#		Part1	:FreezeRayGlowA.part
#		Part2	:FreezeRayIceChuncks2A.part
#		Part3	:FreezeRayIceChuncksA.part
#		Part4	:FreezeRayIceDebris.part
#		Part5	:FreezeRayIceDebris2.part
	End
	
	Event
		EName 	trail
		Type	Start 
		At	WepR
		Magnet	Target
		LookAt	Target
		Bhvr	:MainiceblastFast.bhvr
		Part1	:FreezeRayMistA.part
		Part2	:FreezeRaycrystalStreakA.part
		Part3	:FreezeRaycrystalStreakA.part
		Part4	:FreezeRaycrystalStreakA.part
		Part5	:IceBoltSnowtrail2.part
		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
#		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
	End

End

##############################################################

Condition
	On 	Time
	Time 	33

	Event
		EName 	Prime1
		Type	Start 
		At	WepL
		Bhvr	:MainIceblastFast.bhvr
		Geom	FX_IceBlast
		Magnet	Target
		LookAt	Target
#		Part1	:FreezeRayGlowA.part
#		Part2	:FreezeRayIceChuncks2A.part
#		Part3	:FreezeRayIceChuncksA.part
#		Part4	:FreezeRayIceDebris.part
#		Part5	:FreezeRayIceDebris2.part
	End
	
	Event
		EName 	trail1
		Type	Start 
		At	WepL
		Magnet	Target
		LookAt	Target
		Bhvr	:MainiceblastFast.bhvr
		Part1	:FreezeRayMistA.part
		Part2	:FreezeRaycrystalStreakA.part
		Part3	:FreezeRaycrystalStreakA.part
		Part4	:FreezeRaycrystalStreakA.part
		Part5	:IceBoltSnowtrail2.part
		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
#		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
	End

End

##############################################################


Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime2
		Type	Start 
		At	WepR
		Bhvr	:MainIceblastFast.bhvr
		Geom	FX_IceBlast
		Magnet	Target
		LookAt	Target
#		Part1	:FreezeRayGlowA.part
#		Part2	:FreezeRayIceChuncks2A.part
#		Part3	:FreezeRayIceChuncksA.part
#		Part4	:FreezeRayIceDebris.part
#		Part5	:FreezeRayIceDebris2.part
	End
	
	Event
		EName 	trail2
		Type	Start 
		At	WepR
		Magnet	Target
		LookAt	Target
		Bhvr	:MainiceblastFast.bhvr
		Part1	:FreezeRayMistA.part
		Part2	:FreezeRaycrystalStreakA.part
		Part3	:FreezeRaycrystalStreakA.part
		Part4	:FreezeRaycrystalStreakA.part
		Part5	:IceBoltSnowtrail2.part
		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
#		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
	End

End

##############################################################


Condition
	On 	Time
	Time 	50

	Event
		EName 	Prime3
		Type	Start 
		At	WepL
		Bhvr	:MainIceblastFast.bhvr
		Geom	FX_IceBlast
		Magnet	Target
		LookAt	Target
#		Part1	:FreezeRayGlowA.part
#		Part2	:FreezeRayIceChuncks2A.part
#		Part3	:FreezeRayIceChuncksA.part
#		Part4	:FreezeRayIceDebris.part
#		Part5	:FreezeRayIceDebris2.part
	End
	
	Event
		EName 	trail3
		Type	Start 
		At	WepL
		Magnet	Target
		LookAt	Target
		Bhvr	:MainiceblastFast.bhvr
		Part1	:FreezeRayMistA.part
		Part2	:FreezeRaycrystalStreakA.part
		Part3	:FreezeRaycrystalStreakA.part
		Part4	:FreezeRaycrystalStreakA.part
		Part5	:IceBoltSnowtrail2.part
		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
#		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
	End

End

##############################################################


Condition
	On 	Time
	Time 	57

	Event
		EName 	Prime4
		Type	Start 
		At	WepR
		Bhvr	:MainIceblastFast.bhvr
		Geom	FX_IceBlast
		Magnet	Target
		LookAt	Target
#		Part1	:FreezeRayGlowA.part
#		Part2	:FreezeRayIceChuncks2A.part
#		Part3	:FreezeRayIceChuncksA.part
#		Part4	:FreezeRayIceDebris.part
#		Part5	:FreezeRayIceDebris2.part
	End
	
	Event
		EName 	trail4
		Type	Start 
		At	WepR
		Magnet	Target
		LookAt	Target
		Bhvr	:MainiceblastFast.bhvr
		Part1	:FreezeRayMistA.part
		Part2	:FreezeRaycrystalStreakA.part
		Part3	:FreezeRaycrystalStreakA.part
		Part4	:FreezeRaycrystalStreakA.part
		Part5	:IceBoltSnowtrail2.part
		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
#		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
	End

End

#############################################################


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
		Ename   IceBlastRing
		Type	start
		At	T_chest
		
		#BHVR	Behaviors/OffsetChestToRoot.bhvr

		Part	:StreetTrash.part
		Part	:StreetTrash2.part
		Part	:AOEWindClouds.part
		Part	:AOEWindClouds2.part
		Part	:AOEsnowBurst.part

		LifeSpan	20

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
	
	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	trail
		Type	Destroy
	End
End

Condition
	On	Prime1Hit
		
	Event
		Ename	Prime1
		Type	Destroy
	End

	Event
		Ename	trail1
		Type	Destroy
	End

End

Condition
	On	Prime2Hit
	
	Event
		Ename	Prime2
		Type	Destroy
	End
	
	Event
		Ename	trail2
		Type	Destroy
	End

End

Condition
	On	Prime3Hit
	
	Event
		Ename	Prime3
		Type	Destroy
	End
	
	Event
		Ename	trail3
		Type	Destroy
	End

	Event	
		Ename   IceBlastRing
		Type	start
		At	T_chest
		
		#BHVR	Behaviors/OffsetChestToRoot.bhvr

#		Part	:StreetTrash.part
#		Part	:StreetTrash2.part
#		Part	:AOEWindClouds.part
#		Part	:AOEWindClouds2.part
		Part	:AOEsnowBurst2.part

		LifeSpan	20

	End
End

Condition
	On	Prime4Hit
	
	Event
		Ename	Prime4
		Type	Destroy
	End

	Event
		Ename	trail4
		Type	Destroy
	End
	
#	Event
#		Ename	P5
#		Type	Destroy
#	End
#
#	Event
#		Ename	t5
#		Type	Destroy
#	End
#

End

End				