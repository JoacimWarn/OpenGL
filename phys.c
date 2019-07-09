typedef struct Entity {
    float   pos[2];
    float   vel[2];
    float   acc[2];
} Entity;

static void entity_apply(Entity* this, float fx, float fy) {
    this->acc[0] += fx;
    this->acc[1] += fy;
}

static void entity_update(Entity* this, float t) {
    this->vel[0] += this->acc[0] * t;
    this->vel[1] += this->acc[1] * t;

    this->pos[0] += this->vel[0] * t;
    this->pos[1] += this->vel[1] * t;

    this->acc[0] = 0.0f;
    this->acc[1] = 0.0f;
}