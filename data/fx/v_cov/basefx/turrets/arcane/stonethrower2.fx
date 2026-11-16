#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	90


Input  
	InpName	wepr
End


Input  
	InpName	Target
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Input  
	InpName	Hips
End

###########################################################

Condition
	On	time
	Time	0

	Event
		Type	local
		At	wepr
		Part3	:stonelight.part
		bhvr	:throat.bhvr
		lifespan 120
	end
End
Condition
	On	time
	Time	20

	Event
		EName	Prime
		Type	start
		At	wepr

		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05		
		Part3	:stonedust.part	
#		Part2	POWERS\EarthControl\superStrengthDust1.part
		Part3	:stonepebbles.part
		bhvr	:stone2.bhvr
		Magnet	Target
		LookAt  Target
		lifespan 45
	
	End

	
	Event
		Type	local
		At	wepr
		Part3	:stonebillow.part
		bhvr	:throat.bhvr
		lifespan 120
		
		Sound Grenade1 120.0 120.0 .45
		Sound Grenade1 120.0 120.0 .73
	end

end
	
Condition
	On	time
	Time	23


	Event
		EName	rock
		Type	start
		At	wepr

		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05		
		Part3	:stonedust.part	
#		Part2	POWERS\EarthControl\superStrengthDust1.part
		Part3	:stonepebbles.part
		bhvr	:stone2.bhvr
		Magnet	T_Chest
#		LookAt  Target
		lifespan 45
	
	End

	
	Event
		Type	local
		At	wepr
		Part3	:stonebillow.part
		bhvr	:throat.bhvr
		lifespan 45
		
		Sound Grenade1 120.0 120.0 .45
		Sound Grenade1 120.0 120.0 .73
	end
end
Condition
	On	time
	Time	26


	Event
		EName	rock
		Type	start
		At	wepr

		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05		
		Part3	:stonedust.part	
#		Part2	POWERS\EarthControl\superStrengthDust1.part
		Part3	:stonepebbles.part
		bhvr	:stone2.bhvr
		Magnet	T_Chest
#		LookAt  Target
		lifespan 45
	
	End

	
	Event
		Type	local
		At	wepr
		Part3	:stonebillow.part
		bhvr	:throat.bhvr
		lifespan 45
		
		Sound Grenade1 120.0 120.0 .45
		Sound Grenade1 120.0 120.0 .73
	end
end
Condition
	On	time
	Time	29


	Event
		EName	rock
		Type	start
		At	wepr

		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05		
		Part3	:stonedust.part	
#		Part2	POWERS\EarthControl\superStrengthDust1.part
		Part3	:stonepebbles.part
		bhvr	:stone2.bhvr
		Magnet	T_Chest
#		LookAt  Target
		lifespan 45
	
	End

	
	Event
		Type	local
		At	wepr
		Part3	:stonebillow.part
		bhvr	:throat.bhvr
		lifespan 45
		
		Sound Grenade1 120.0 120.0 .45
		Sound Grenade1 120.0 120.0 .73
	end
end
Condition
	On	time
	Time	32


	Event
		EName	rock
		Type	start
		At	wepr

		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05		
		Part3	:stonedust.part	
#		Part2	POWERS\EarthControl\superStrengthDust1.part
		Part3	:stonepebbles.part
		bhvr	:stone2.bhvr
		Magnet	T_Chest
#		LookAt  Target
		lifespan 45
	
	End

	
	Event
		Type	local
		At	wepr
		Part3	:stonebillow.part
		bhvr	:throat.bhvr
		lifespan 45
		
		Sound Grenade1 120.0 120.0 .45
		Sound Grenade1 120.0 120.0 .73
	end
end


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End
	Event
		EName 	Rock
		Type	Destroy
	End
	Event
		EName 	Rock
		Type	Destroy
	End
	Event
		EName 	Rock
		Type	Destroy
	End
	Event
		EName 	Rock
		Type	Destroy
	End
	Event
		EName 	Rock
		Type	Destroy
	End

End


End			