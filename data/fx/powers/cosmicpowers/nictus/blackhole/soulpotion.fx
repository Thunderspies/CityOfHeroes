#########################################################
##	Fireweapon
########################################################
FxInfo
LifeSpan	30
##########################################
##  AUDIO
##########################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		#Sound	rumble2_loop 80 60 .5

	End
End

######################################################################


Condition
	On 	Time
	Time 	0

	Event
		
		Type	Local 
		At	hips
		
		Part	POWERS\CosmicPowers\Nictus\BlasterShapShift\NovaTendrilsBuildUp.part
		Part	POWERS\CosmicPowers\Nictus\BlasterShapShift\NovaTendrilsBuildUpblue.part
		Part	POWERS\CosmicPowers\Nictus\BlasterShapShift\NovaTendrilsBuildUpPurple.part
		
		lifespan	10
	End

End

Condition
	On 	Time
	Time 	6

	Event
		
		Type	Local 
		At	hips
		
		#Bhvr	Behaviors/CameraShake01.bhvr
		Part	POWERS\CosmicPowers\Nictus\BlasterShapShift\NovaTendrils.part
		Part	POWERS\CosmicPowers\Nictus\BlasterShapShift\NovaTendrilsBlue.part
		Part	POWERS\CosmicPowers\Nictus\BlasterShapShift\NovaTendrilsPurple.part
		Part	POWERS\CosmicPowers\Nictus\BlasterShapShift\InfernorockBits2.part
		Part2	POWERS\CosmicPowers\Nictus\BlasterShapShift\Inferno.part
		Part2	POWERS\CosmicPowers\Nictus\BlasterShapShift\InfernoBlue.part
		Part2	POWERS\CosmicPowers\Nictus\BlasterShapShift\InfernoPurple.part
		
		lifespan	10
	End


End	

#############################################################################################3

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hurricane
		Type	local 
		At	root
		BHVR	Behaviors/PortalPivot.bhvr
		Part	:BlackHole.part
		Part	:BlackHolePurple.part
		Part	:BlackHoleBlue.part
		Part	:BlackHoleOutterRings.part
		Part	:BlackHoleOutterRingsIn.part
		Part	:PortalThickness.part
		Part	:InwardPortalMatter.part
		Part	:BlackHoleCore.part
		LifeSpan	20
	End


End

End
##################################

