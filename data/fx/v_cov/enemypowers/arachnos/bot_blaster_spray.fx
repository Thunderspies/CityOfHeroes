#########################################################
##	
##

FxInfo
LifeSpan  100

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################

Condition
	On 	Time
	Time 	0 #38


	Event
		ENAME   cannon
		Type	local
		At	hair
		bhvr	:Bot_Blaster_off.bhvr
#		#geom    Testing_Target
#		Part1	:Bot_ARShell.Part
#		Sound	Assault3 120.0 30.0 1.0
#		magnet	t_chest
#		lifespan 3
	End


	Event
		ename	genspot
		Type	local
		At	waist
		Sound ArachnosBot_servo3 120.0 120.0 1.0

		bhvr	:Bot_Blaster_Genspot.bhvr
	end

	Event
		ename	gens
		Type	local
		At	waist

		bhvr	:Bot_Blaster_Gens.bhvr
		part	:bot_blaster_gensparks.part
		#geom    Testing_Target
	end

	Event
		Type	local
		At	cannon
		part	:bot_blaster_chargering.part
		lifespan 55
	end

	Event
		ENAME   down
		Type	local
		At	root

		bhvr	down.bhvr
	end


end

Condition
	On 	time
	Time 	38

Event
		Ename	Prime
		Type	start
		At	cannon
		bhvr	:Bot_Blaster_Spray_Prime.bhvr
		magnet	t_chest
		#geom    Testing_Target

end

Event
		Type	positonly
		At	cannon
		Part1	:Bot_Blaster_Spray_Bits.part
		lookat  T_Chest

end


	Event
		Type	local
		At	cannon

#		Part1	:Bot_Blaster_ring.part
		Part1	:Bot_Blaster_flameB.part
		pmagnet root
		lifespan 7
	end

	Event
		Type	local
		At	genspot
		part1	:Bot_Blaster_ringSmoke.part
		part1	:Bot_Blaster_ringSmokeN.part
		part1	:Bot_Blaster_ringSteam.part
		part1	:Bot_Blaster_ringSteamN.part
		pmagnet root
	end

End	
	


Condition
	On 	time
	Time 	42

Event
		Ename	Prime1
		Type	start
		At	cannon
		bhvr	:Bot_Blaster_Spray_Prime.bhvr
		magnet	t_chest
		#geom    Testing_Target

end

Event
		Type	positonly
		At	cannon
		Part1	:Bot_Blaster_Spray_Bits.part
		lookat  T_Chest

end


	Event
		Type	local
		At	cannon

#		Part1	:Bot_Blaster_ring.part
		Part1	:Bot_Blaster_flameB.part
		pmagnet root
		lifespan 7
	end

	Event
		Type	local
		At	genspot
		part1	:Bot_Blaster_ringSmoke.part
		part1	:Bot_Blaster_ringSmokeN.part
#		part1	:Bot_Blaster_ringSteam.part
#		part1	:Bot_Blaster_ringSteamN.part
		pmagnet root
	end

End	
	


Condition
	On 	time
	Time 	46

Event
		Ename	Prime2
		Type	start
		At	cannon
		bhvr	:Bot_Blaster_Spray_Prime.bhvr
		magnet	t_chest
		#geom    Testing_Target

end

Event
		Type	positonly
		At	cannon
		Part1	:Bot_Blaster_Spray_Bits.part
		lookat  T_Chest

end


	Event
		Type	local
		At	cannon

#		Part1	:Bot_Blaster_ring.part
		Part1	:Bot_Blaster_flameB.part
		pmagnet root
		lifespan 7
	end

	Event
		Type	local
		At	genspot
		part1	:Bot_Blaster_ringSmoke.part
		part1	:Bot_Blaster_ringSmokeN.part
#		part1	:Bot_Blaster_ringSteam.part
#		part1	:Bot_Blaster_ringSteamN.part
		pmagnet root
	end

End	
	


Condition
	On 	primehit
	
	Event
		EName 	Prime
		Type	Destroy
		

	End
end


Condition
	On 	prime1hit
	
	Event
		EName 	Prime1
		Type	Destroy
		

	End
end


Condition
	On 	prime2hit
	
	Event
		EName 	Prime2
		Type	Destroy
		

	End
end