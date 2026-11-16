#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	wepL
End

Lifespan 160

##################################


Condition
	On	Time
	Time	0
	
	Event
		EName	Pod
		Type	Local
		At	WepL
		Geom	GEO_WepL_MediPod
		Bhvr	Behaviors/MediPodFadeIn.bhvr
		Part	:mglow.part
	End

End

##########################################
##  AUDIO
##########################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		#Sound	Rumble2 80 60 .5

	End
End

######################################################################

Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local 
		At	hips
		Sound Midnight_Visage_01 100.0 100.0 .9
	End


End

Condition
	On 	Time
	Time 	30

	Event
		
		Type	Local 
		At	hips
		
		Part	POWERS\CosmicPowers\Nictus\BlasterShapshift\NovaTendrilsBuildUp.part
		Part	POWERS\CosmicPowers\Nictus\BlasterShapshift\NovaTendrilsBuildUpblue.part
		Part	POWERS\CosmicPowers\Nictus\BlasterShapshift\NovaTendrilsBuildUpPurple.part
		
		lifespan	90
	End

End

Condition
	On 	Time
	Time 	36

	Event
		
		Type	Local 
		At	hips
		
		#Bhvr	Behaviors/CameraShake01.bhvr
		Part	POWERS\CosmicPowers\Nictus\BlasterShapshift\NovaTendrils.part
		Part	POWERS\CosmicPowers\Nictus\BlasterShapshift\NovaTendrilsBlue.part
		Part	POWERS\CosmicPowers\Nictus\BlasterShapshift\NovaTendrilsPurple.part
		Part	POWERS\CosmicPowers\Nictus\BlasterShapshift\InfernorockBits2.part
		Part2	POWERS\CosmicPowers\Nictus\BlasterShapshift\Inferno.part
		Part2	POWERS\CosmicPowers\Nictus\BlasterShapshift\InfernoBlue.part
		Part2	POWERS\CosmicPowers\Nictus\BlasterShapshift\InfernoPurple.part
		
		lifespan	90
	End


End	

Condition
	On 	Time
	Time 	55

	Event
		Type	Local 
		At	hips
		
		Bhvr	Behaviors/CameraShake01.bhvr
		lifespan	90
	End


End


######################################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	blackhole
		Type	local 
		At	root
		BHVR	Behaviors/PortalPivot.bhvr
		Part	POWERS\CosmicPowers\Nictus\BlackHole\BlackHole.part
		Part	POWERS\CosmicPowers\Nictus\BlackHole\BlackHolePurple.part
		Part	POWERS\CosmicPowers\Nictus\BlackHole\BlackHoleBlue.part
		Part	POWERS\CosmicPowers\Nictus\BlackHole\BlackHoleOutterRings.part
		Part	POWERS\CosmicPowers\Nictus\BlackHole\BlackHoleOutterRingsIn.part
		Part	POWERS\CosmicPowers\Nictus\BlackHole\PortalThickness.part
		Part	POWERS\CosmicPowers\Nictus\BlackHole\InwardPortalMatter.part
		Part	POWERS\CosmicPowers\Nictus\BlackHole\BlackHoleCore.part
		LifeSpan	30

	End


End

End