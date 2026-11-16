#########################################################
## Lord Recluse - Arachnos PTT Buff
#########################################################

FxInfo

Lifespan	360

#########################################################
## Building
#########################################################

Condition
	On	Time
	Time	31

	Event
		Type	Start
		At	Chest

		Part1	:Buildup_FinalContractingRing1.part
		Part2	:Buildup_FinalContractingRing2.part
		Sound Seer2 80 80 .77
		
		LifeSpan	30
	End

End

Condition
	On	Time
	Time	16

	Event
		
		Type	Posit
		At	Chest
		Part2	:Buildup_HitRaysBig.part
		lifespan	75
	End
	
End

Condition
	On	Time
	Time	76

	Event
		
		Type	Posit
		At	Chest
		Part2	:Buildup_HitRaysBig.part
		Part4	:Buildup_HitStreaksBig.part
		lifespan	45
	End
	
End

Condition
	On	Time
	Time	106

	Event
		
		Type	Posit
		At	Chest
		Part2	:Buildup_HitRaysBig.part
		Part4	:Buildup_HitStreaksBig.part
		lifespan	15
	End
	
End


Condition
	On	Time
	Time	106

	Event
		Type	Start
		At	Chest

		Part3	:Buildup_DiffuseGlow.part
		Part2	:Buildup_AdditiveGlow.part
		#Sound	Infuse4 80 33 .6
		
		LifeSpan	45
	End
	
End

Condition
	On	Time
	Time	106

	Event
		Type	Start
		At	Chest

		ChildFX	:LordReclusePTTBuff_Aura.fx
		LifeSpan	60
	End
	
End



#########################################################
## Explosion
#########################################################

Condition
	On	Time
	Time	186

	Event
		
		Type	start
		At	root
		Sound PCM_explo 300 300 .93
		Bhvr	:CameraShake.bhvr
		lifespan	31
	End
End

Condition
	On	Time
	Time	181

	Event
		
		Type	start
		At	chest
	
		Part1	:Explosion_ThinFastRings.part
		Part2	:Explosion_BigLightRays1.part
		lifespan	61
	End

	Event
		
		Type	start
		At	chest
	
		Part2	:Explosion_BigLightRays2.part
		lifespan	46
	End

	Event
		
		Type	start
		At	Root
	
		Part1	:Explosion_Cloud.part
		Part2	:Explosion_CloudDecal.part
		Part3	:Explosion_Cloud_Additive.part
		Part4	:Explosion_CloudDecal_Additive.part
		lifespan	16
	End
End

Condition
	On	Time
	Time	186

	Event
		
		Type	start
		At	chest
	
		Part1	:Explosion_Streaks.part
		lifespan	61	#10
	End
End	

Condition
	On	Time
	Time	210

	Event
		
		Type	start
		At	root
	
		childFX		:LordReclusePTTBuff_Aura.fx
		#lifespan	61	#10
	End
End	


End