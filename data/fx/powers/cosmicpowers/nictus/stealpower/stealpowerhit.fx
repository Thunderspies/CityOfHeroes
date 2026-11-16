#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	chest
End

Lifespan	50

########################################################



Condition
	On 	Time
	Time 	0

	Event
		ENAME   Tendrills
		Type	Local 
		At	chest
		Part	:SoulDrainHitPuddle2.part
		Part	:DamageHitMist2.part
		Part	:SoulDrainHitTendril2.part
		Part	:SoulDrainHitTendrilPurple.part
		Part	:SoulDrainHitTendrilBlue.part
		Part	:RedRays.part
		Sound WarshadeHit2 80 80 .96
		LifeSpan	30
	End

End

End
##################################

