#########################################################
##	General red electricity crawling across characters limbs
########################################################
FxInfo

LifeSpan	60

########################################################


Condition

	On 	time
	Time 	0

	Event
		Ename	lift
		Type	local

		At	wepL
#		bhvr	V_COV\EnemyPowers\Arachnos\Mu_HandLightningFx\mu_StaticSpot.bhvr
		Sound StaticDischarge2 90 90 .9
		lifespan 200
		#geom    Testing_Target
	End

	Event
		Type	start
		At	lift
		childfx	:StaticDischarge_Child2.fx
		lifespan 50
	end

	Event
		Type	start
		At	lift
		childfx	:StaticDischarge_Child2.fx
		lifespan 50
	end

	Event
		Type	start
		At	lift
		childfx	:StaticDischarge_Child2.fx
		lifespan 50
	end


end

Condition

	On 	time
	Time 	0
event
		Type	positonly
		At	Hips
		Sound eleccontrol2 100 100 .5
end
end

Condition

	On 	cycle
	Time 	7

	Event
		Type	start
		At	lift
		childfx	:StaticDischarge_Child2.fx
		lifespan 50
	end

end
Condition

	On 	cycle
	Time 	3

	Event
		Type	start
		At	lift
		childfx	:StaticDischarge_Child2.fx
		lifespan 50
	end

end
Condition

	On 	cycle
	Time 	5

	Event
		Type	start
		At	lift
		childfx	:StaticDischarge_Child2.fx
		lifespan 50
	end

end
#######################################
