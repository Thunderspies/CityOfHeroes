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
	Time 	9
	
	Event
		Type	Local
		At 	origin
		Sound Arachnosbot_blast1 100 100 .8
	End
End

Condition
	On 	Time
	Time 	39
	
	Event
		Type	Local
		At 	origin
		Sound robotlaserred2 100 100 1.0
	End
End


Condition
	On 	Time
	Time 	0 #38


	Event
		ENAME   cannon
		Type	local
		At	hair
		bhvr	:Bot_Blaster_off.bhvr
#		geom    Testing_Target
#		Part1	:Bot_ARShell.Part
		
#		magnet	t_chest
#		lifespan 3
	End


	Event
		ename	genspot
		Type	local
		At	waist

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


end

Condition
	On 	time
	Time 	38




	Event
		ENAME   prime
		Type	start
		At	cannon
		bhvr	:Bot_Blaster.bhvr
#		geom    Testing_Target
		Part1	:Bot_Blaster_BallTail.part
		Part1	:Bot_Blaster_BallGlow.part
		Part1	:Bot_Blaster_BallStar.part
		Part1	:Bot_Blaster_SteamTrail.part
		magnet	t_chest
		lifespan 60
#		lifespan 3
	End


	Event
		Type	local
		At	cannon

		Part1	:Bot_Blaster_ring.part
	end


	Event
		ENAME   screw
		Type	local
		At	prime

		bhvr	:Bot_Blaster_Twist.bhvr
	end

	Event
		Type	local
		At	screw

		bhvr	:Bot_Blaster_TwistOff.bhvr
		Part1	:Bot_Blaster_Twist.part
		Part1	:Bot_Blaster_Twist.part
		lifespan 60
	end


	Event
		Type	local
		At	screw

		bhvr	:Bot_Blaster_TwistOff2.bhvr
		Part1	:Bot_Blaster_Twist.part
		Part1	:Bot_Blaster_Twist.part
		lifespan 60
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
		ENAME   prime1
		Type	start
		At	cannon
		bhvr	:Bot_Blaster.bhvr
#		geom    Testing_Target
		Part1	:Bot_Blaster_BallTail.part
		Part1	:Bot_Blaster_BallGlow.part
		Part1	:Bot_Blaster_BallStar.part
		Part1	:Bot_Blaster_SteamTrail.part
		magnet	t_head
		lifespan 60
#		lifespan 3
	End


	Event
		Type	local
		At	cannon

		Part1	:Bot_Blaster_ring.part
		Part1	:Bot_Blaster_flameB.part
		pmagnet root
		lifespan 7
	end


	Event
		ENAME   screw1
		Type	local
		At	prime1

		bhvr	:Bot_Blaster_Twist.bhvr
	end

	Event
		Type	local
		At	screw1

		bhvr	:Bot_Blaster_TwistOff.bhvr
		Part1	:Bot_Blaster_Twist.part
		Part1	:Bot_Blaster_Twist.part
		lifespan 60
	end


	Event
		Type	local
		At	screw1

		bhvr	:Bot_Blaster_TwistOff2.bhvr
		Part1	:Bot_Blaster_Twist.part
		Part1	:Bot_Blaster_Twist.part
		lifespan 60
	end
	Event
		Type	local
		At	genspot
		part1	:Bot_Blaster_ringSmoke.part
		part1	:Bot_Blaster_ringSmokeN.part
		pmagnet root
	end


End		

Condition
	On 	time
	Time 	46




	Event
		ENAME   prime2
		Type	start
		At	cannon
		bhvr	:Bot_Blaster.bhvr
#		geom    Testing_Target
		Part1	:Bot_Blaster_BallTail.part
		Part1	:Bot_Blaster_BallGlow.part
		Part1	:Bot_Blaster_BallStar.part
		Part1	:Bot_Blaster_SteamTrail.part
		magnet	t_waist
		lifespan 60
#		lifespan 3
	End


	Event
		Type	local
		At	cannon

		Part1	:Bot_Blaster_ring.part
		Part1	:Bot_Blaster_flameB.part
		pmagnet root
		lifespan 7
	end


	Event
		ENAME   screw2
		Type	local
		At	prime2

		bhvr	:Bot_Blaster_Twist.bhvr
	end

	Event
		Type	local
		At	screw2

		bhvr	:Bot_Blaster_TwistOff.bhvr
		Part1	:Bot_Blaster_Twist.part
		Part1	:Bot_Blaster_Twist.part
		lifespan 60
	end


	Event
		Type	local
		At	screw2

		bhvr	:Bot_Blaster_TwistOff2.bhvr
		Part1	:Bot_Blaster_Twist.part
		Part1	:Bot_Blaster_Twist.part
		lifespan 60
	end
	Event
		Type	local
		At	genspot
		part1	:Bot_Blaster_ringSmoke.part
		part1	:Bot_Blaster_ringSmokeN.part
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