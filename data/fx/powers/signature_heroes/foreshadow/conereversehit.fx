#########################################################
##	Fireweapon
########################################################
FxInfo

LifeSpan	245

###################################################################################################
##Blast
########################
#
#Condition
#	On	Time
#	Time	10
#
#	Event
#		EName	Streak1
#		Type	local
#		At	Chest
#
#		ChildFx	POWERS\Signature_Heroes\ForeShadow\ForeShadowHit.fx
#		
#		LifeSpan	45
#	
#	End
#
#End

Condition

	Event
		EName	Prime
		Type	start
		At	T_Chest
		
		Bhvr	Behaviors/ForeShadowBlurProjectiles.bhvr
		
		Part	:BlastTendrilsInvert.part
		Part	:BlastTendrils.part
		Part	:BlastTendrilsInvertBright.part
		Part	:BlastTendrilsBright.part

		Magnet	BendMystic

	End

End


Condition
	On 	PrimeHit
	
	Event
		ChildFx	POWERS\Signature_Heroes\ForeShadow\ForeShadowHit.fx
		
	End

	Event
		EName 	Prime
		Type	Destroy
		
	End

End

End

